#include "solver_grid_fluid.h"

HinaPE::GridSystemData3::GridSystemData3(const HinaPE::GridSystemData3::Opt &opt)
{
	_velocity = std::make_shared<Geom::FaceCenteredVectorGrid3>();
	_velocity_index = _advectable_vector_data.size();
	_advectable_vector_data.push_back(_velocity);
	resize(opt.resolution, opt.grid_spacing, opt.origin);
}
void HinaPE::GridSystemData3::resize(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin)
{
	_opt.resolution = resolution;
	_opt.grid_spacing = grid_spacing;
	_opt.origin = origin;

	for (auto &data: _scalar_data)
		data->resize(resolution, grid_spacing, origin, Constant::Zero);
	for (auto &data: _vector_data)
		data->resize(resolution, grid_spacing, origin, mVector3::Zero());
	for (auto &data: _advectable_scalar_data)
		data->resize(resolution, grid_spacing, origin, Constant::Zero);
	for (auto &data: _advectable_vector_data)
		data->resize(resolution, grid_spacing, origin, mVector3::Zero());
}

HinaPE::GridFluidSolver::GridFluidSolver(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin)
{
	// _grid_data
	_grids = std::make_shared<GridSystemData3>();
	_grids->resize(resolution, grid_spacing, origin);
	_opt.fix_sub_time_step = false;

	// sub_solvers
	_advection_solver = std::make_shared<CubicSemiLagrangian3>();
	_diffusion_solver = std::make_shared<GridBackwardEulerDiffusionSolver3>();
	_pressure_solver = std::make_shared<GridFractionalSinglePhasePressureSolver3>();
	_opt.fix_sub_time_step = false;
}
void HinaPE::GridFluidSolver::_compute_external_forces(float dt)
{
	// gravity
	auto vel = _grids->_velocity;
	if (_opt.gravity.x() > Constant::Epsilon)
		vel->parallel_for_each_u_index([&](size_t i, size_t j, size_t k) { vel->u(i, j, k) += _opt.gravity.x() * dt; });
	if (_opt.gravity.y() > Constant::Epsilon)
		vel->parallel_for_each_u_index([&](size_t i, size_t j, size_t k) { vel->v(i, j, k) += _opt.gravity.y() * dt; });
	if (_opt.gravity.z() > Constant::Epsilon)
		vel->parallel_for_each_u_index([&](size_t i, size_t j, size_t k) { vel->w(i, j, k) += _opt.gravity.z() * dt; });

	_apply_boundary_condition();
}
void HinaPE::GridFluidSolver::_compute_viscosity(float dt)
{
}
void HinaPE::GridFluidSolver::_compute_pressure(float dt)
{
	if (_pressure_solver != nullptr)
	{
		auto vel = _grids->_velocity;
		auto vel0 = _grids->_velocity->clone();

//		_pressure_solver->solve(vel0, dt, vel,

		_apply_boundary_condition();
	}
}
void HinaPE::GridFluidSolver::_compute_advection(float dt)
{
}
void HinaPE::GridFluidSolver::_update_collider(float dt) { if (_collider) _collider->update(PhysicsAnimation::_opt._current_time, dt); }
void HinaPE::GridFluidSolver::_update_emitter(float dt) { if (_emitter) _emitter->update(PhysicsAnimation::_opt._current_time, dt); }
void HinaPE::GridFluidSolver::_apply_boundary_condition()
{
//	auto vel = _grids->_velocity;
//	if (vel != nullptr && )
}
void HinaPE::GridFluidSolver::VALID_CHECK()
{
	if (_grids->_opt.resolution.x == 0 || _grids->_opt.resolution.y == 0 || _grids->_opt.resolution.z == 0)
		throw std::runtime_error("Resolution is not set");

	if (_emitter == nullptr)
		throw std::runtime_error("Emitter is not set");

	_emitter->VALID_CHECK();
}
