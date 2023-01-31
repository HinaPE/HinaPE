#ifndef HINAPE_SOLVER_H
#define HINAPE_SOLVER_H

#include "animation/physics_animation.h"
#include "data.h"
#include "emitter.h"

#include "geometry/collider.h"

namespace Hina
{
class GridFluidSolver : public PhysicsAnimation
{
public:
	struct Opt
	{
		// Env
		mVector3 gravity = mVector3(0, -9.8, 0);

		// fluid params
		real viscosity = 0.0;
		real max_cfl = 5.0;
	} _opt;

public:
	GridFluidSolver(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin);

protected:
	void on_physics_init() final;
	void on_physics_update(float dt) final;

	virtual void on_before_physics_update(float dt) {}; // empty now
	virtual void on_after_physics_update(float dt) {}; // empty now
	virtual void compute_external_forces(float dt);
	virtual void compute_viscosity(float dt);
	virtual void compute_pressure(float dt);
	virtual void compute_advection(float dt);

private:
	void before_physics_update(float dt);
	void after_physics_update(float dt);
	void update_collider(float dt);
	void update_emitter(float dt);

	void apply_boundary_condition();

private:
	GridSystemData3Ptr _grids;
	GridEmitter3Ptr _emitter;
	ColliderPtr _collider;
};
}

#endif //HINAPE_SOLVER_H
