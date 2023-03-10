#include "pcisph_solver.h"

void HinaPE::PCISPHSolver::update(real dt) const
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

void HinaPE::PCISPHSolver::_emit_particles() const
{
}

void HinaPE::PCISPHSolver::_accumulate_force() const
{
}

void HinaPE::PCISPHSolver::_time_integration() const
{
}

void HinaPE::PCISPHSolver::_resolve_collision() const
{
}

void HinaPE::PCISPHSolver::INSPECT()
{
	ImGui::Text("Solver Inspector");
	ImGui::Text("Particles: %zu", _data->_positions.size());
	INSPECT_REAL(_opt.gravity[1], "g");
	ImGui::Separator();
}

void HinaPE::PCISPHSolver::VALID_CHECK() const
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
