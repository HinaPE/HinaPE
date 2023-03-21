#include "pbf_solver_new.h"

// ============================================================================================================
// ================================================== Solver ==================================================
void HinaPE::PBFSolverNew::init()
{
	// init data
	if (_data == nullptr)
		_data = std::make_shared<Data>();
	if (_domain == nullptr)
		_domain = std::make_shared<BoxDomain>();
	if (_emitter == nullptr)
		_emitter = std::make_shared<VolumeParticleEmitter3>();

	_init_fluid_particles();
	_init_boundary_particles();
}

void HinaPE::PBFSolverNew::update(real dt) const
{
	// algorithm line 1~4
	_apply_force_and_predict_position();

	// algorithm line 5~7
	_update_neighbor();

	// algorithm line 8~19
	_solve_density_constraints();

	// algorithm line 20~24
	_update_positions_and_velocities();

	// simple collision handling here (temporary)
	_resolve_collision();
}

void HinaPE::PBFSolverNew::_init_fluid_particles() const
{
	std::vector<mVector3> init_pos, init_vel;
	_emitter->emit(&init_pos, &init_vel);
	_data->add_fluid(init_pos, init_vel);
	// TODO: update mass
}

void HinaPE::PBFSolverNew::_init_boundary_particles() const
{
	std::vector<mVector3> target_boundary;

	static const auto add_wall = [&](const mVector3 &minX, const mVector3 &maxX)
	{
		const real diam = 2.0 * _opt.radius;
		const int stepsX = (int) ((maxX.x() - minX.x()) / diam) + 1;
		const int stepsY = (int) ((maxX.y() - minX.y()) / diam) + 1;
		const int stepsZ = (int) ((maxX.z() - minX.z()) / diam) + 1;

		// TODO: parallelize
		for (int i = 0; i < stepsX; ++i)
		{
			for (int j = 0; j < stepsY; ++j)
			{
				for (int k = 0; k < stepsZ; ++k)
				{
					const real x = minX.x() + i * diam;
					const real y = minX.y() + j * diam;
					const real z = minX.z() + k * diam;
					target_boundary.emplace_back(x, y, z);
				}
			}
		}
	};

	const auto half_width = _domain->_extent.x();
	const auto half_height = _domain->_extent.y();
	const auto half_depth = _domain->_extent.z();

	const real x1 = -half_width;
	const real x2 = half_width;
	const real y1 = -half_height;
	const real y2 = half_height;
	const real z1 = -half_depth;
	const real z2 = half_depth;

	add_wall(mVector3(x1, y1, z1), mVector3(x2, y1, z2)); // floor
	add_wall(mVector3(x1, y2, z1), mVector3(x2, y2, z2)); // top
	add_wall(mVector3(x1, y1, z1), mVector3(x1, y2, z2)); // left
	add_wall(mVector3(x2, y1, z1), mVector3(x2, y2, z2)); // right
	add_wall(mVector3(x1, y1, z1), mVector3(x2, y2, z1)); // back
	add_wall(mVector3(x1, y1, z2), mVector3(x2, y2, z2)); // front

	_data->add_boundary(target_boundary);
}

void HinaPE::PBFSolverNew::_apply_force_and_predict_position() const
{
	_data->Fluid.predicted_position = _data->Fluid.positions;

	auto fluid_size = _data->fluid_size();
	auto &p = _data->Fluid.predicted_position;
	auto &v = _data->Fluid.velocities;
	auto &f = _data->Fluid.forces;
	const auto &m = _data->Fluid.mass;
	const auto &g = _opt.gravity;
	const auto &dt = _opt.current_dt;

	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
	{
		// Gravity Forces
		mVector3 gravity = m * g;
		f[i] = gravity;

		// semi-implicit Euler
		v[i] += dt * f[i] / m;
		p[i] += dt * v[i];
	});
}

void HinaPE::PBFSolverNew::_update_neighbor() const
{
	auto size = _data->fluid_size();
	auto &p = _data->Fluid.predicted_position;
	auto &nl = _data->NeighborList;

	StdKernel poly6(_opt.radius);
	PointHashGridSearch3 searcher(_opt.radius);
	searcher.build(p);
	Util::parallelFor(Constant::ZeroSize, size, [&](size_t i)
	{
		auto origin = p[i];
		nl[i].clear();
		searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				nl[i].push_back(j);
		});
	});
}

void HinaPE::PBFSolverNew::_solve_density_constraints() const
{
	for (int i = 0; i < _opt.constraint_solver_iterations; ++i)
	{
		// Note:
		// "i" is the index of the current particle,
		// "j" is the index of the neighbor particle
	}
}

void HinaPE::PBFSolverNew::_update_positions_and_velocities() const
{
	auto &x = _data->Fluid.positions;
	auto &v = _data->Fluid.velocities;
	const auto &p = _data->Fluid.predicted_position;
	const auto dt = _opt.current_dt;

	// First, update velocities
	Util::parallelFor(Constant::ZeroSize, x.size(), [&](size_t i)
	{
		v[i] = (p[i] - x[i]) / dt;
	});

	// Finally, update positions
	Util::parallelFor(Constant::ZeroSize, x.size(), [&](size_t i)
	{
		x[i] = p[i];
	});
}

void HinaPE::PBFSolverNew::_resolve_collision() const
{
	auto fluid_size = _data->fluid_size();
	auto &x = _data->Fluid.positions;
	auto &v = _data->Fluid.velocities;
	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
	{
		_domain->resolve_collision(_opt.radius, _opt.restitution, &x[i], &v[i]);
	});
}

void HinaPE::PBFSolverNew::_update_density() const
{
	auto &d = _data->Fluid.densities;
	const auto &p = _data->Fluid.predicted_position;
	const auto &m = _data->Fluid.mass;
	const auto size = _data->fluid_size();
	const auto &nl = _data->NeighborList;

	StdKernel poly6(_opt.radius);
	Util::parallelFor(Constant::ZeroSize, size, [&](size_t i)
	{
		real sum = poly6(0); // self density
		for (int j = 0; j < nl[i].size(); ++j)
		{
			real dist = (p[i] - p[nl[i][j]]).length();
			sum += poly6(dist);
		}
		d[i] = m * sum; // rho(x) = m * sum(W(x - xj))
	});
}

void HinaPE::PBFSolverNew::INSPECT()
{
}
// ================================================== Solver ==================================================
// ============================================================================================================


// ==========================================================================================================
// ================================================== Data ==================================================
HinaPE::PBFSolverNew::Data::Data()
{
	track(&Fluid.positions);
}
auto HinaPE::PBFSolverNew::Data::fluid_size() const -> size_t { return Fluid.positions.size(); }
auto HinaPE::PBFSolverNew::Data::boundary_size() const -> size_t { return Boundary.positions.size(); }
void HinaPE::PBFSolverNew::Data::add_fluid(const std::vector<mVector3> &positions, const std::vector<mVector3> &velocities)
{
	if (positions.size() != velocities.size())
		throw std::runtime_error("positions.size() != velocities.size()");

	auto size = positions.size();
	Fluid.positions.insert(Fluid.positions.end(), positions.begin(), positions.end());
	Fluid.velocities.insert(Fluid.velocities.end(), velocities.begin(), velocities.end());
	Fluid.predicted_position.insert(Fluid.predicted_position.end(), positions.begin(), positions.end());
	Fluid.forces.insert(Fluid.forces.end(), size, mVector3::Zero());
	Fluid.densities.insert(Fluid.densities.end(), size, 0.0);
	Fluid.lambdas.insert(Fluid.lambdas.end(), size, 0.0);
	Fluid.delta_p.insert(Fluid.delta_p.end(), size, mVector3::Zero());
	NeighborList.insert(NeighborList.end(), size, std::vector<unsigned int>());
}
void HinaPE::PBFSolverNew::Data::add_boundary(const std::vector<mVector3> &positions)
{
	Boundary.positions.insert(Boundary.positions.end(), positions.begin(), positions.end());
}
// ================================================== Data ==================================================
// ==========================================================================================================
