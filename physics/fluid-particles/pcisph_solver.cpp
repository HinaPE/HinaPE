#include "pcisph_solver.h"

void HinaPE::PCISPHSolver::update(real dt) const
{
}
void HinaPE::PCISPHSolver::INSPECT()
{
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
void HinaPE::PCISPHSolver::Data::_update_poses()
{
	static real size = 0.03;
	_poses.resize(_positions.size());
	for (size_t i = 0; i < _positions.size(); ++i)
		_poses[i] = Kasumi::Pose(_positions[i], {}, {size, size, size});
	_dirty = true;
}
