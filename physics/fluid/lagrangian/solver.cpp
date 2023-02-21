#include "solver.h"

HinaPE::SPHSolver::SPHSolver()
{
	_data = std::make_shared<Data>();
	_kernel = std::make_shared<StdKernel>();
	_emitter = std::make_shared<PointParticleEmitter3>();
	dynamic_cast<StdKernel *>(_kernel.get())->_opt.kernel_radius = _data->_opt.kernel_radius;
	dynamic_cast<StdKernel *>(_kernel.get())->_rebuild_();
}
void HinaPE::SPHSolver::init()
{
	_scene->add(_emitter);
	_scene->add(_data);
	inspect(_scene.get());
}
void HinaPE::SPHSolver::step(real dt)
{
	_opt.current_dt = dt;

	// begin
//	_update_collider();
	_update_emitter();

	// kernels
//	_build_neighbor();
//	_update_density();

	_accumulate_force();
	_time_integration();

	_data->update();
}
void HinaPE::SPHSolver::_accumulate_force() const
{
	auto &x = _data->_positions;
	auto &v = _data->_velocities;
	auto &f = _data->_forces;
	auto &d = _data->_densities;
	auto &p = _data->_pressures;
	const auto &m = _data->_opt.mass;

	// external
	Util::parallelFor(Constant::ZeroSize, _data->size(), [&](size_t i)
	{
		mVector3 gravity = m * _opt.gravity;
		f[i] = gravity;
	});

//	// viscosity
//	Util::parallelFor(Constant::ZeroSize, _data->size(), [&](size_t i)
//	{
//		const auto &neighbors = _data->_neighbor_lists[i];
//		for (size_t j: neighbors)
//		{
//			real dist = (x[i] - x[j]).length();
//			f[i] += _opt.viscosity_coefficient * m * m * (v[j] - v[i]) / d[j] * (*_kernel).second_derivative(dist);
//		}
//	});
//
//	// pressure
//	_update_pressure();
//	Util::parallelFor(Constant::ZeroSize, _data->size(), [&](size_t i)
//	{
//		const auto &neighbors = _data->_neighbor_lists[i];
//		for (size_t j: neighbors)
//		{
//			real dist = (x[i] - x[j]).length();
//			if (dist > HinaPE::Constant::Epsilon)
//			{
//				mVector3 dir = (x[i] - x[j]) / dist;
//				f[i] += -m * m * (p[i] / (d[i] * d[i]) + p[j] / (d[j] * d[j])) * (*_kernel).gradient(dist, dir);
//			}
//		}
//	});
}
void HinaPE::SPHSolver::_time_integration() const
{
	auto &x = _data->_positions;
	auto &v = _data->_velocities;
	auto &f = _data->_forces;
	const auto &m = _data->_opt.mass;
	const auto &dt = _opt.current_dt;

	Util::parallelFor(Constant::ZeroSize, _data->size(), [&](size_t i)
	{
		v[i] += dt * f[i] / m;
		x[i] += dt * v[i];
	});
}
void HinaPE::SPHSolver::_resolve_collision() const
{
}
void HinaPE::SPHSolver::_resize_buffer()
{
	_new_positions.resize(_data->size());
	_new_velocities.resize(_data->size());
}
void HinaPE::SPHSolver::_build_neighbor() const
{
	_data->build_neighbor();
}
void HinaPE::SPHSolver::_update_collider() const
{
}
void HinaPE::SPHSolver::_update_emitter() const
{
	_emitter->emit(_data->_positions, _data->_velocities);
	_data->resize();
}
void HinaPE::SPHSolver::_update_pressure() const
{
	auto &d = _data->_densities;
	auto &p = _data->_pressures;
	const real target_density = _data->_opt.target_density;
	const real eos_scale = target_density * _opt.speed_of_sound * _opt.speed_of_sound;
	const real eos_exponent = _opt.eos_exponent;
	const real negative_pressure_scale = _opt.negative_pressure_scale;
	Util::parallelFor(Constant::ZeroSize, _data->size(), [&](size_t i)
	{
		// See Murnaghan-Tait equation of state from
		// https://en.wikipedia.org/wiki/Tait_equation
		p[i] = eos_scale / eos_exponent * (std::pow(d[i] / target_density, eos_exponent) - 1.0);

		if (p[i] < 0)
			p[i] *= negative_pressure_scale;
	});
}
void HinaPE::SPHSolver::_update_density() const
{
	auto &p = _data->_positions;
	auto &d = _data->_densities;
	const auto &m = _data->_opt.mass;

	Util::parallelFor(Constant::ZeroSize, _data->size(), [&](size_t i)
	{
		mVector3 origin = p[i];
		real sum = 0;
		_data->_neighbor_searcher->for_each_nearby_point(origin, _data->_opt.kernel_radius, [&](size_t, const mVector3 &neighbor_position)
		{
			real dist = (origin - neighbor_position).length();
			sum += (*_kernel)(dist); // Note: Don't use parallel for here
		});
		d[i] = m * sum;
	});
}
void HinaPE::SPHSolver::INSPECT()
{
	ImGui::Text("Solver");
	ImGui::Text("Solver %zu", _data->_positions.size());
	ImGui::Separator();
	INSPECT_VEC3(_opt.gravity, "gravity");
	INSPECT_REAL(_opt.eos_exponent, "eos exponent");
	INSPECT_REAL(_opt.negative_pressure_scale, "negative pressure scale");
	INSPECT_REAL(_opt.viscosity_coefficient, "viscosity coefficient");
	INSPECT_REAL(_opt.pseudo_viscosity_coefficient, "pseudo viscosity coefficient");
	INSPECT_REAL(_opt.speed_of_sound, "speed of sound");
	INSPECT_REAL(_opt.time_step_limit_scale, "time step limit scale");
	_emitter->INSPECT();

	if (!_opt.inited)
	{
		if (ImGui::Button("Generate"))
		{
			_opt.inited = true;
		}
	}
	ImGui::Separator();
	ImGui::Separator();
}


void HinaPE::SPHSolver::Data::build_neighbor()
{
	_neighbor_searcher = std::make_shared<PointSimpleListSearcher3>();
	_neighbor_searcher->build(_positions);

	_neighbor_lists.clear();
	_neighbor_lists.resize(_positions.size());
	for (int i = 0; i < _positions.size(); ++i)
	{
		mVector3 origin = _positions[i];
		_neighbor_lists[i].clear();

		_neighbor_searcher->for_each_nearby_point(origin, _opt.max_search_radius, [&](size_t j, const mVector3 &)
		{
			if (i != j)
				_neighbor_lists[i].push_back(j);
		});
	}
}
void HinaPE::SPHSolver::Data::update()
{
	ParticlesObject::_opt.poses.clear();

	Object3D::_opt.dirty = true;

	for (auto &pos: _positions)
	{
		Kasumi::Pose pose;
		pose.position = pos;
		pose.scale = 0.02 * mVector3::One();
		ParticlesObject::_opt.poses.push_back(pose);
	}

	_rebuild_();
}
auto HinaPE::SPHSolver::Data::size() const -> size_t
{
	if (_positions.size() != _velocities.size() && _positions.size() != _forces.size() && _positions.size() != _densities.size() && _positions.size() != _pressures.size())
		throw std::runtime_error("SPHSolver::Data::size() error");
	return _positions.size();
}
void HinaPE::SPHSolver::Data::resize()
{
	if (_positions.size() != _velocities.size())
		throw std::runtime_error("SPHSolver::Data::size() error");

	auto size = _positions.size();
	_forces.resize(size);
	_densities.resize(size);
	_pressures.resize(size);
}


void HinaPE::StdKernel::_rebuild_()
{
	h = _opt.kernel_radius;
	h2 = h * h;
	h3 = h2 * h;
	h5 = h3 * h2;
}
auto HinaPE::StdKernel::operator()(real distance) const -> real
{
	if (distance * distance >= h2)
		return 0;

	real x = 1.0 - distance * distance / h2;
	return 315.0 / (64.0 * HinaPE::Constant::PI * h3) * x * x * x;
}
auto HinaPE::StdKernel::first_derivative(real distance) const -> real
{
	if (distance > h)
		return 0;

	real x = 1.0 - distance * distance / h2;
	return -945.0 / (32.0 * HinaPE::Constant::PI * h5) * distance * x * x;
}
auto HinaPE::StdKernel::second_derivative(real distance) const -> real
{
	if (distance * distance > h2)
		return 0;

	double x = distance * distance / h2;
	return 945.0 / (32.0 * HinaPE::Constant::PI * h5) * (1 - x) * (5 * x - 1);
}
auto HinaPE::StdKernel::gradient(const mVector3 &point) const -> mVector3
{
	real dist = point.length();
	if (dist > 0)
		return gradient(dist, point / dist);
	return mVector3::Zero();
}
auto HinaPE::StdKernel::gradient(real distance, const mVector3 &direction) const -> mVector3
{
	return -first_derivative(distance) * direction;
}
