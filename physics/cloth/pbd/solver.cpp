#include "solver.h"
#include "backends/timer.h"

HinaPE::PBDClothSolver::PBDClothSolver() : _current_dt(0.2f)
{
	_data = std::make_shared<ClothObject>(5, 3, 30, 30);
	_prepare();
}

void HinaPE::PBDClothSolver::update(real dt)
{
	_current_dt = dt;
	HINA_TRACK(_external_force(), "external force");
	HINA_TRACK(_time_integration(), "time integration");
	HINA_TRACK(_constraint_projection(), "constraint projection");
	HINA_TRACK(_update_state(), "update state");
}

void HinaPE::PBDClothSolver::_prepare()
{
	if (_opt.distance_constraint)
		_constraints.emplace_back(std::make_shared<DistanceConstraint>(_data->_positions, _data->_inv_masses, _data->_init_edges));
}

void HinaPE::PBDClothSolver::_external_force() const
{
	for (auto &_force: _data->_forces)
		_force += mVector3(0.f, _opt.gravity, 0.f);
}

void HinaPE::PBDClothSolver::_time_integration() const
{
	auto &positions = _data->_positions;
	auto &velocities = _data->_velocities;
	auto &forces = _data->_forces;
	auto &inv_masses = _data->_inv_masses;

	// update velocity
	for (auto i = 0; i < positions.size(); ++i)
	{
		if (inv_masses[i] == 0.f)
			continue;
		velocities[i] += _opt.damping * forces[i] * inv_masses[i] * _current_dt;
	}

	// update position
	for (auto i = 0; i < positions.size(); ++i)
		positions[i] += velocities[i] * _current_dt;

	// simple collision
	for (auto i = 0; i < positions.size(); ++i)
		if (positions[i].y() < -6.3)
		{
			positions[i].y() = -6.3;
			velocities[i].y() = 0.f;
		}
}

void HinaPE::PBDClothSolver::_constraint_projection()
{
	for (auto &c: _constraints)
		c->solve();
}

void HinaPE::PBDClothSolver::_update_state() const
{
	auto &positions = _data->_positions;
	auto &pre_positions = _data->_pre_positions;
	auto &velocities = _data->_velocities;

	// update velocity
	for (auto i = 0; i < positions.size(); ++i)
	{
		velocities[i] = (positions[i] - pre_positions[i]) / _current_dt;
		pre_positions[i] = positions[i];
	}

	// clear forces
	auto &forces = _data->_forces;
	std::fill(forces.begin(), forces.end(), mVector3(0.f, 0.f, 0.f));
}
void HinaPE::PBDClothSolver::INSPECT() {}
