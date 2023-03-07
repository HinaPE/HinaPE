#include "sph_solver.h"

void HinaPE::SPHSolver::update(real dt) const
{
	// emit particle to data, and rebuild data
	_emit_particles();

	// accumulate external forces, viscosity force and pressure force
	_accumulate_force();

	// do semi-euler integration
	_time_integration();

	// deal with collision (particle-solid)
	_resolve_collision();
}

void HinaPE::SPHSolver::_emit_particles() const
{
	_emitter->emit(&_data->_positions, &_data->_velocities);
	_data->_forces.resize(_data->_positions.size(), mVector3::Zero());
	_data->_densities.resize(_data->_positions.size(), 0);
	_data->_pressures.resize(_data->_positions.size(), 0);
	_data->_update_neighbor();
	_data->_update_density();
	_data->_update_pressure();
}

void HinaPE::SPHSolver::_accumulate_force() const
{
	auto &x = _data->_positions;
	auto &v = _data->_velocities;
	auto &f = _data->_forces;
	auto &d = _data->_densities;
	auto &p = _data->_pressures;
	const auto &m = _data->_mass;

	// Non Pressure Forces
	Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
	{
		mVector3 gravity = m * _opt.gravity;
		f[i] = gravity;
	});

//	StdKernel kernel(_data->kernel_radius);
//	Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
//	{
//		const auto &neighbors = _data->_neighbor_lists[i];
//		for (size_t j: neighbors)
//		{
//			real dist = (x[i] - x[j]).length();
//			f[i] += _opt.viscosity_coefficient * m * m * (v[j] - v[i]) / d[j] * kernel.second_derivative(dist);
//		}
//	});

//	// Pressure Forces
//	Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
//	{
//		const auto &neighbors = _data->_neighbor_lists[i];
//		for (size_t j: neighbors)
//		{
//			real dist = (x[i] - x[j]).length();
//			if (dist > HinaPE::Constant::Epsilon)
//			{
//				mVector3 dir = (x[i] - x[j]) / dist;
//				f[i] += -m * m * (p[i] / (d[i] * d[i]) + p[j] / (d[j] * d[j])) * kernel.gradient(dist, dir);
//			}
//		}
//	});
}

void HinaPE::SPHSolver::_time_integration() const
{
	auto &x = _data->_positions;
	auto &v = _data->_velocities;
	auto &f = _data->_forces;
	const auto &m = _data->_mass;
	const auto &dt = _opt.current_dt;

	// semi-euler integration
	Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
	{
		v[i] += dt * f[i] / m;
		x[i] += dt * v[i];
	});
}

void HinaPE::SPHSolver::_resolve_collision() const
{
	// collide with domain
	Util::parallelFor(Constant::ZeroSize, _data->_positions.size(), [&](size_t i)
	{
		_domain->resolve_collision(_data->_radius, _opt.restitution, &_data->_positions[i], &_data->_velocities[i]);
	});
}

void HinaPE::SPHSolver::INSPECT()
{
	ImGui::Text("Solver Inspector");
	ImGui::Text("Particles: %zu", _data->_positions.size());
	ImGui::Separator();
	INSPECT_VEC3(_opt.gravity, "gravity");
	INSPECT_REAL(_opt.eos_exponent, "eos exponent");
	INSPECT_REAL(_opt.negative_pressure_scale, "negative pressure scale");
	INSPECT_REAL(_opt.viscosity_coefficient, "viscosity coefficient");
	INSPECT_REAL(_opt.pseudo_viscosity_coefficient, "pseudo viscosity coefficient");
	INSPECT_REAL(_opt.speed_of_sound, "speed of sound");
	INSPECT_REAL(_opt.time_step_limit_scale, "time step limit scale");
//	_emitter->INSPECT();

	if (!_opt.inited)
		if (ImGui::Button("Generate"))
			_opt.inited = true;
	ImGui::Separator();
}

void HinaPE::SPHSolver::VALID_CHECK() const
{
	if (_data == nullptr) throw std::runtime_error("SPHSolver::_data is nullptr");
	if (_domain == nullptr) throw std::runtime_error("SPHSolver::_domain is nullptr");
	if (_emitter == nullptr) throw std::runtime_error("SPHSolver::_emitter is nullptr");

	if (_data->_positions.size() != _data->_velocities.size() &&
		_data->_positions.size() != _data->_forces.size() &&
		_data->_positions.size() != _data->_densities.size() &&
		_data->_positions.size() != _data->_pressures.size())
		throw std::runtime_error("SPHSolver::_data size mismatch");
}

void HinaPE::SPHSolver::Data::_update_poses()
{
	static real size = 0.03;
	_poses.resize(_positions.size());
	for (size_t i = 0; i < _positions.size(); ++i)
		_poses[i] = Kasumi::Pose(_positions[i], {}, {size, size, size});
	_dirty = true;
}

void HinaPE::SPHSolver::Data::_update_neighbor()
{
	_neighbor_search = std::make_shared<PointSimpleListSearch3>();
	_neighbor_search->build(_positions);
	_neighbor_lists.resize(_positions.size());

	auto &x = _positions;
	for (size_t i = 0; i < _positions.size(); ++i)
	{
		mVector3 origin = x[i];
		_neighbor_lists[i].clear();

		_neighbor_search->for_each_nearby_point(origin, kernel_radius, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				_neighbor_lists[i].push_back(j);
		});
	}
}

void HinaPE::SPHSolver::Data::_update_density()
{
	auto &x = _positions;
	auto &d = _densities;
	const auto &m = _mass;

	Util::parallelFor(Constant::ZeroSize, _positions.size(), [&](size_t i)
	{
		real sum = 0;
		StdKernel kernel(kernel_radius);
		_neighbor_search->for_each_nearby_point(x[i], kernel_radius, [&](size_t, const mVector3 &p)
		{
			real dist = (x[i] - p).length();
			sum += kernel(dist);
		});
		d[i] = m * sum;
	});
}

void HinaPE::SPHSolver::Data::_update_pressure()
{
	auto &d = _densities;
	auto &p = _pressures;
	const real eos_scale = target_density * 100 * 100; // speed of sound = 100
	const real eos_exponent = eos_exponent;
	const real negative_pressure_scale = negative_pressure_scale;
	Util::parallelFor(Constant::ZeroSize, _positions.size(), [&](size_t i)
	{
		// See Murnaghan-Tait equation of state from
		// https://en.wikipedia.org/wiki/Tait_equation
		p[i] = eos_scale / eos_exponent * (std::pow(d[i] / target_density, eos_exponent) - 1.0);

		if (p[i] < 0)
			p[i] *= negative_pressure_scale;
	});
}
