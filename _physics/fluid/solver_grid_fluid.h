#ifndef HINAPE_SOLVER_GRID_FLUID_H
#define HINAPE_SOLVER_GRID_FLUID_H

#include "animation/physics_animation.h"
#include "geom/grid.h"
#include "geom/collider.h"
#include "emitter/emitter.h"

#include "sub_solver/grid_diffusion_solver.h"
#include "sub_solver/grid_pressure_solver.h"
#include "sub_solver/grid_boundary_condition_solver.h"
#include "sub_solver/advection_solver.h"

namespace HinaPE
{
class GridSystemData3
{
public:
	void resize(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin);

public:
	struct Opt
	{
		mSize3 resolution;
		mVector3 grid_spacing;
		mVector3 origin;
	} _opt;

public:
	explicit GridSystemData3() = default;
	explicit GridSystemData3(const Opt &opt);
	GridSystemData3(const GridSystemData3 &other) = delete;
	auto operator=(const GridSystemData3 &other) -> GridSystemData3 & = delete;
	GridSystemData3(GridSystemData3 &&other) = delete;
	auto operator=(GridSystemData3 &&other) -> GridSystemData3 & = delete;
	~GridSystemData3() = default;

public:
	// velocity
	Geom::FaceCenteredVectorGrid3Ptr _velocity;
	size_t _velocity_index = 0;

private:
	std::vector<Geom::ScalarGrid3Ptr> _scalar_data;
	std::vector<Geom::VectorGrid3Ptr> _vector_data;
	std::vector<Geom::ScalarGrid3Ptr> _advectable_scalar_data;
	std::vector<Geom::VectorGrid3Ptr> _advectable_vector_data;
};
using GridSystemData3Ptr = std::shared_ptr<GridSystemData3>;

class GridFluidSolver : public Animation::PhysicsAnimation
{
public:
	void add_emitter(const GridEmitter3Ptr &emitter) { _emitter = emitter; } // TODO: support multiple emitters in the future
	void add_collider(const Geom::ColliderPtr &collider) { _collider = collider; } // TODO: support multiple colliders in the future

protected:
	void on_physics_init() final
	{
		_update_collider(Constant::Zero);
		_update_emitter(Constant::Zero);
	}
	void on_physics_update(float dt) final
	{
		_before_physics_update(dt);

		_compute_external_forces(dt);
		_compute_viscosity(dt);
		_compute_pressure(dt);
		_compute_advection(dt);

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
	virtual void _compute_external_forces(float dt);
	virtual void _compute_viscosity(float dt);
	virtual void _compute_pressure(float dt);
	virtual void _compute_advection(float dt);

public:
	struct Opt : PhysicsAnimation::Opt
	{
		// Env
		mVector3 gravity = mVector3(0, -9.8, 0);

		// fluid params
		real viscosity = 0.0;
		real max_cfl = 5.0;
	} _opt;
	void VALID_CHECK() final;

public:
	GridFluidSolver(const mSize3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin);

private:
	void _update_collider(float dt);
	void _update_emitter(float dt);
	void _apply_boundary_condition();

private:
	GridSystemData3Ptr _grids;
	GridEmitter3Ptr _emitter;
	Geom::ColliderPtr _collider;

	// solvers
	AdvectionSolver3Ptr _advection_solver;
	GridPressureSolver3Ptr _pressure_solver;
	GridDiffusionSolver3Ptr _diffusion_solver;
	GridBoundaryConditionSolver3Ptr _boundary_condition_solver;
};
}
#endif //HINAPE_SOLVER_GRID_FLUID_H
