#include "pbd_solver.h"

HinaPE::Cloth::PBDSolver::PBDSolver(const HinaPE::Cloth::PBDSolver::Opt &opt) : ClothSolver(opt), _opt(opt) { setup_pbd(); }
void HinaPE::Cloth::PBDSolver::step(real dt)
{
	_current_dt = dt;
	external_force();
	time_integration();
	constraint_projection();
	update_state();
}
void HinaPE::Cloth::PBDSolver::setup_pbd()
{
	if (_opt.distance_constraint)
	{
		DistanceConstraint::Opt opt;
		opt.stiffness = _opt.distance_constraint_stiffness;
		_constraints.emplace_back(std::make_shared<DistanceConstraint>(opt, _cloth_data->_positions, _cloth_data->_inv_masses, _cloth_data->_init_edges));
	}
}
void HinaPE::Cloth::PBDSolver::external_force()
{
	auto &forces = _cloth_data->_forces;
	for (auto i = 0; i < forces.size(); ++i)
		forces[i] += mVector3(0.f, _opt.gravity, 0.f);
}
void HinaPE::Cloth::PBDSolver::time_integration()
{
	auto &positions = _cloth_data->_positions;
	auto &velocities = _cloth_data->_velocities;
	auto &forces = _cloth_data->_forces;
	auto &inv_masses = _cloth_data->_inv_masses;

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
		if (positions[i].y < -6.3)
		{
			positions[i].y = -6.3;
			velocities[i].y = 0.f;
		}
}
void HinaPE::Cloth::PBDSolver::constraint_projection()
{
	for (auto &c: _constraints)
		c->solve();
}
void HinaPE::Cloth::PBDSolver::update_state()
{
	auto &positions = _cloth_data->_positions;
	auto &pre_positions = _cloth_data->_pre_positions;
	auto &velocities = _cloth_data->_velocities;

	// update velocity
	for (auto i = 0; i < positions.size(); ++i)
	{
		velocities[i] = (positions[i] - pre_positions[i]) / _current_dt;
		pre_positions[i] = positions[i];
	}

	// clear forces
	auto &forces = _cloth_data->_forces;
	std::fill(forces.begin(), forces.end(), mVector3(0.f, 0.f, 0.f));
}
