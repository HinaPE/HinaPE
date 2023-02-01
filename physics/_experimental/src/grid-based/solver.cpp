#include "solver.h"

Hina::GridFluidSolver::GridFluidSolver(const Hina::Base::Size3 &resolution, const Hina::mVector3 &grid_spacing, const Hina::mVector3 &origin)
{
	_grids = std::make_shared<GridSystemData3>();
	_grids->resize(resolution, grid_spacing, origin);
	_opt.fix_sub_time_step = false;
}
void Hina::GridFluidSolver::on_physics_init()
{
	update_collider(Constant::Zero);
	update_emitter(Constant::Zero);
}
void Hina::GridFluidSolver::on_physics_update(float dt)
{
	if (_grids->_opt.resolution.x == 0 || _grids->_opt.resolution.y == 0 || _grids->_opt.resolution.z == 0)
		return; // empty grid

	before_physics_update(dt);
	compute_external_forces(dt);
	compute_viscosity(dt);
	compute_pressure(dt);
	compute_advection(dt);
	after_physics_update(dt);
}
void Hina::GridFluidSolver::compute_external_forces(float dt)
{
	// gravity
	auto vel = _grids->_velocity;
	if (_opt.gravity.x() > Constant::Epsilon)
		vel->parallel_for_each_u_index([&](size_t i, size_t j, size_t k) { vel->u(i, j, k) += _opt.gravity.x() * dt; });
	if (_opt.gravity.y() > Constant::Epsilon)
		vel->parallel_for_each_u_index([&](size_t i, size_t j, size_t k) { vel->v(i, j, k) += _opt.gravity.y() * dt; });
	if (_opt.gravity.z() > Constant::Epsilon)
		vel->parallel_for_each_u_index([&](size_t i, size_t j, size_t k) { vel->w(i, j, k) += _opt.gravity.z() * dt; });

	apply_boundary_condition();
}
void Hina::GridFluidSolver::compute_viscosity(float dt)
{
}
void Hina::GridFluidSolver::compute_pressure(float dt)
{
	if (_pressure_solver != nullptr)
	{
		auto vel = _grids->_velocity;
		auto vel0 = _grids->_velocity->clone();

//		_pressure_solver->solve(vel0, dt, vel,

		apply_boundary_condition();
	}
}
void Hina::GridFluidSolver::compute_advection(float dt)
{
}
void Hina::GridFluidSolver::before_physics_update(float dt)
{
	update_collider(dt);
	update_emitter(dt);
	apply_boundary_condition();
}
void Hina::GridFluidSolver::after_physics_update(float dt) {}
void Hina::GridFluidSolver::update_collider(float dt) { if (_collider) _collider->update(PhysicsAnimation::_opt._current_time, dt); }
void Hina::GridFluidSolver::update_emitter(float dt) { if (_emitter) _emitter->update(PhysicsAnimation::_opt._current_time, dt); }
void Hina::GridFluidSolver::apply_boundary_condition()
{
//	auto vel = _grids->_velocity;
//	if (vel != nullptr && )
}
