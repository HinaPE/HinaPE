#include "sph_solver.h"

void HinaPE::SPHSolver::update(real dt) const
{
	// emit particle to data
	_emit_particles();

	_data->_update_density();
	_data->_update_pressure();

	// accumulate external forces and pressure force
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

	// Pressure Forces
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
	for (int i = 0; i < _positions.size(); ++i)
		_poses[i] = Kasumi::Pose(_positions[i], {}, {size, size, size});
	_dirty = true;
}
void HinaPE::SPHSolver::Data::_update_density()
{
}
void HinaPE::SPHSolver::Data::_update_pressure()
{
}
