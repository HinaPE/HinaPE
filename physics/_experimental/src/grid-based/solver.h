#ifndef HINAPE_SOLVER_H
#define HINAPE_SOLVER_H

#include "animation/physics_animation.h"
#include "geometry/collider.h"
#include "data.h"
#include "emitter.h"

#include "advection_solver.h"
#include "grid_pressure_solver.h"
#include "grid_diffusion_solver.h"
#include "grid_boundary_condition_solver.h"

namespace Hina
{
class GridFluidSolver : public PhysicsAnimation
{
protected:
	void on_physics_init() final
	{
		_update_collider(Constant::Zero);
		_update_emitter(Constant::Zero);
	}
	void on_physics_update(float dt) final
	{
		_before_physics_update(dt);

		compute_external_forces(dt);
		compute_viscosity(dt);
		compute_pressure(dt);
		compute_advection(dt);

		_after_physics_update(dt);
	}
	void _before_physics_update(float dt)
	{
		_update_collider(dt);
		_update_emitter(dt);
		_apply_boundary_condition();

		on_before_physics_update(dt);
	}
	void _after_physics_update(float dt)
	{
		on_after_physics_update(dt);
	}
	virtual void on_before_physics_update(float dt) {};
	virtual void on_after_physics_update(float dt) {};

protected:
	virtual void compute_external_forces(float dt);
	virtual void compute_viscosity(float dt);
	virtual void compute_pressure(float dt);
	virtual void compute_advection(float dt);

public:
	struct Opt : PhysicsAnimation::Opt
	{
		// Env
		mVector3 gravity = mVector3(0, -9.8, 0);

		// fluid params
		real viscosity = 0.0;
		real max_cfl = 5.0;
	} _opt;

public:
	GridFluidSolver(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin);

private:
	void _update_collider(float dt);
	void _update_emitter(float dt);
	void _apply_boundary_condition();
	auto ___VALID_CHECK___() -> bool final;

private:
	GridSystemData3Ptr _grids;
	GridEmitter3Ptr _emitter;
	ColliderPtr _collider;

	// solvers
	AdvectionSolver3Ptr _advection_solver;
	GridPressureSolver3Ptr _pressure_solver;
	GridDiffusionSolver3Ptr _diffusion_solver;
	GridBoundaryConditionSolver3Ptr _boundary_condition_solver;
};
}

#endif //HINAPE_SOLVER_H
