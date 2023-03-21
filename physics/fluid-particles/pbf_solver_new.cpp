#include "pbf_solver_new.h"

#define S HinaPE::Math::to_string
#define Record(x, y) _data->debug_info[i].emplace_back(x + S(y))

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
	_update_neighbor();
	_update_density();
}

void HinaPE::PBFSolverNew::update(real dt) const
{
	_data->debug_info.clear();
	_data->debug_info.resize(_data->fluid_size());

	// algorithm line 1~4
	_apply_force_and_predict_position();

	// algorithm line 5~7
	_update_neighbor();

	// algorithm line 8~19
	_solve_density_constraints();

	// algorithm line 20~24
	_update_positions_and_velocities();
}

void HinaPE::PBFSolverNew::_init_fluid_particles() const
{
	std::vector<mVector3> init_pos, init_vel;
	_emitter->emit(&init_pos, &init_vel);
	_data->add_fluid(init_pos, init_vel);

	// update mass
	std::vector<std::vector<unsigned int>> temp_neighbor_list;
	temp_neighbor_list.resize(init_pos.size());
	PointHashGridSearch3 searcher(_opt.kernel_radius);
	searcher.build(init_pos);

	Util::parallelFor(Constant::ZeroSize, init_pos.size(), [&](size_t i)
	{
		auto origin = init_pos[i];
		temp_neighbor_list[i].clear();
		searcher.for_each_nearby_point(origin, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				temp_neighbor_list[i].push_back(j);
		});
	});

	StdKernel poly6(_opt.kernel_radius);
	real max_number_density = 0;
	for (int i = 0; i < init_pos.size(); ++i)
	{
		real sum = poly6(0); // self density
		const auto &point = init_pos[i];
		for (const auto &neighbor_point_id: temp_neighbor_list[i])
		{
			auto dist = (point - init_pos[neighbor_point_id]).length();
			sum += poly6(dist);
		}
		max_number_density = std::max(max_number_density, sum);
	}

	if (max_number_density > 0)
		_data->Fluid.mass = std::max((_opt.target_density / max_number_density), HinaPE::Constant::Zero);
	else
		throw std::runtime_error("max_number_density is zero");
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
	// Update Target: predicted_position, velocities, forces
	_data->Fluid.predicted_position = _data->Fluid.positions;

	auto &p = _data->Fluid.predicted_position;
	auto &v = _data->Fluid.velocities;
	auto &f = _data->Fluid.forces;
	const auto fluid_size = _data->fluid_size();
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
	// Update Target: NeighborList
	auto &nl = _data->NeighborList;
	const auto fluid_size = _data->fluid_size();
	const auto &p = _data->Fluid.predicted_position;

	PointHashGridSearch3 searcher(_opt.kernel_radius);
	searcher.build(p);
	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
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
	// Update Target: Lambdas, Delta P
	for (int i = 0; i < _opt.constraint_solver_iterations; ++i)
	{
		// Note:
		// "i" is the index of the current particle,
		// "j" is the index of the neighbor particle

		_update_density();

		const auto fluid_size = _data->fluid_size();
		const auto d0 = _opt.target_density;
		const auto h = _opt.radius;
		const auto eps = 1e-6;

		const auto &p = _data->Fluid.predicted_position;
		const auto &d = _data->Fluid.densities;
		const auto &nl = _data->NeighborList;
		const auto &m = _data->Fluid.mass;
		StdKernel poly6(_opt.kernel_radius);

		auto &lambdas = _data->Fluid.lambdas;
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			real d_i = d[i];
			real C_i = d_i / d0 - 1;

			if (C_i > 0) // if density is bigger than water density, do constraints projection
			{
				real sum_grad_C_i_squared = 0;
				mVector3 grad_C_i = mVector3::Zero();

				for (const auto j: nl[i])
				{
					const auto p_i = p[i];
					const auto p_j = p[j];
					const mVector3 grad_C_j = -(m / d0) * poly6.gradient(p_i - p_j);

					// Equation (8)
					sum_grad_C_i_squared += grad_C_j.length_squared();
					grad_C_i -= grad_C_j;
				}

				sum_grad_C_i_squared += grad_C_i.length_squared();

				// Equation (11): compute lambda
				real lambda = -C_i / (sum_grad_C_i_squared + eps); // eps is for soft constraint
				lambdas[i] = lambda; // thread safe write

				// for debug
				Record("C: ", C_i);
				Record("Sum Grad C Squared: ", sum_grad_C_i_squared);
				Record("Grad C: ", grad_C_i);
				Record("Lambda: ", lambda);
			}
		});

		auto &dp = _data->Fluid.delta_p;
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			const auto &lambda_i = lambdas[i];

			auto k_corr = m * 1.0e-04;
			auto n_corr = 4.0;
			auto q_corr = 0.1;

			// Equation (12): compute delta p
			mVector3 delta_p_i = mVector3::Zero();
			for (const auto j: nl[i])
			{
				const auto &lambda_j = lambdas[j];
				const auto p_i = p[i];
				const auto p_j = p[j];

				const auto w_corr = poly6(q_corr * h);
				const auto ratio = poly6((p_i - p_j).length()) / w_corr;
				const auto s_corr = -k_corr * pow(ratio, n_corr);

				const mVector3 grad_C_j = -(m / d0) * poly6.gradient(p_i - p_j);
				delta_p_i -= (lambda_i + lambda_j + s_corr) * grad_C_j;
			}
			dp[i] = delta_p_i; // thread safe write

			Record("delta p: ", delta_p_i);
		});

		// Finally, apply delta p to all particles
		auto &p_to_write = _data->Fluid.predicted_position;
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			p_to_write[i] -= dp[i];
		});

		// Dead simple collision
		const auto &v = _data->Fluid.velocities;
		Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
		{
			auto temp_v = v[i]; // we don't need to update velocity here
			_domain->resolve_collision(_opt.radius, _opt.restitution, &p_to_write[i], &temp_v);
		});
	}
}

void HinaPE::PBFSolverNew::_update_positions_and_velocities() const
{
	const auto &p = _data->Fluid.predicted_position;
	const auto &d = _data->Fluid.densities;
	const auto &nl = _data->NeighborList;
	const auto m = _data->Fluid.mass;
	const auto fluid_size = _data->fluid_size();
	const auto dt = _opt.current_dt;
	StdKernel poly6(_opt.kernel_radius);

	auto &x = _data->Fluid.positions;
	auto &v = _data->Fluid.velocities;
	// First, update velocities
	Util::parallelFor(Constant::ZeroSize, x.size(), [&](size_t i)
	{
		v[i] = (p[i] - x[i]) / dt;
	});

	// Apply XSPH viscosity
	const auto c = _opt.viscosity;
	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
	{
		const auto &p_i = p[i];
		const auto &v_i = v[i];

		mVector3 sum_value = mVector3::Zero();
		for (auto const &j: nl[i])
		{
			const real d_j = d[j];
			const auto &p_j = p[j];
			const auto &v_j = v[j];
			mVector3 tmp = v_i - v_j;
			tmp *= poly6((p_i - p_j).length()) * (m / d_j);
			sum_value += tmp;
		}

		v[i] = v_i - c * sum_value;

		Record("XSPH Vis: ", c * sum_value);
	});

	Util::parallelFor(Constant::ZeroSize, fluid_size, [&](size_t i)
	{
		const auto &p_i = p[i];
		const auto &v_i = v[i];

		mVector3 f_vorticity = mVector3::Zero();
		mVector3 N = mVector3::Zero();
		mVector3 curl = mVector3::Zero();
		mVector3 curl_x = mVector3::Zero();
		mVector3 curl_y = mVector3::Zero();
		mVector3 curl_z = mVector3::Zero();

		for (auto const &j: nl[i])
		{
			const auto &p_j = p[j];
			const auto &v_j = v[j];
			mVector3 tmp = v_j - v_i;
			curl += tmp.cross(poly6.gradient(p_i - p_j));
			curl_x += tmp.cross(poly6.gradient(p_i + mVector3(0.01, 0, 0) - p_j));
			curl_y += tmp.cross(poly6.gradient(p_i + mVector3(0, 0.01, 0) - p_j));
			curl_z += tmp.cross(poly6.gradient(p_i + mVector3(0, 0, 0.01) - p_j));
		}

		real curlLen = curl.length();
		N.x() = curl_x.length() - curlLen;
		N.y() = curl_y.length() - curlLen;
		N.z() = curl_z.length() - curlLen;
		N = N.normalized();
		f_vorticity = 0.00001 * N.cross(curl);

		v[i] = v_i + f_vorticity * dt;

		Record("Vorticity: ", f_vorticity * dt);
	});

	// Finally, update positions
	Util::parallelFor(Constant::ZeroSize, x.size(), [&](size_t i)
	{
		x[i] = p[i];
	});
}

void HinaPE::PBFSolverNew::_update_density() const
{
	// Update Target: densities
	auto &d = _data->Fluid.densities;
	const auto &p = _data->Fluid.predicted_position;
	const auto &m = _data->Fluid.mass;
	const auto size = _data->fluid_size();
	const auto &nl = _data->NeighborList;

	StdKernel poly6(_opt.kernel_radius);
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
	ImGui::Text("SOLVER INSPECTOR");
	ImGui::Text("Fluids: %zu", _data->fluid_size());
	ImGui::Text("Boundaries: %zu", _data->boundary_size());
	INSPECT_REAL(_opt.gravity[1], "g");
	ImGui::Separator();

	auto inst_id = _data->_inst_id;
	if (inst_id >= 0 && inst_id < _data->fluid_size())
	{
		ImGui::Text("Inst: %d", inst_id);
		ImGui::Text("Mass: %f", _data->Fluid.mass);
		ImGui::Text("Force: {%.3f, %.3f, %.3f}", _data->Fluid.forces[inst_id].x(), _data->Fluid.forces[inst_id].y(), _data->Fluid.forces[inst_id].z());
		ImGui::Text("Density: %.3f", _data->Fluid.densities[inst_id]);
		ImGui::Text("Lambda: %.3f", _data->Fluid.lambdas[inst_id]);
		ImGui::Text("Neighbors: %zu", _data->NeighborList[inst_id].size());
		ImGui::Separator();
		if (_data->debug_info.empty())
			return;
		for (auto &info: _data->debug_info[inst_id])
		{
			ImGui::Text("%s\n", info.c_str());
			ImGui::Separator();
		}
	}
}
// ================================================== Solver ==================================================
// ============================================================================================================


// ==========================================================================================================
// ================================================== Data ==================================================
HinaPE::PBFSolverNew::Data::Data()
{
	track(&Fluid.positions);
	_color_map = &color_map;
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

	color_map.insert(color_map.end(), size, Color::ORANGE);
	debug_info.insert(debug_info.end(), size, std::vector<std::string>());
}
void HinaPE::PBFSolverNew::Data::add_boundary(const std::vector<mVector3> &positions)
{
	Boundary.positions.insert(Boundary.positions.end(), positions.begin(), positions.end());
}
// ================================================== Data ==================================================
// ==========================================================================================================
