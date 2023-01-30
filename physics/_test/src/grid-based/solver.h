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

		real viscosity = 0.0;
		real max_cfl = 5.0;
	} _opt;

public:
	GridFluidSolver(const Base::Size3 &resolution, const mVector3 &grid_spacing, const mVector3 &origin);

protected:
	void on_physics_init() final;
	void on_physics_update(float dt) final;
	void update_collider(float dt);
	void update_emitter(float dt);

private:
	GridSystemData3Ptr _grids;
	GridEmitter3Ptr _emitter;
	ColliderPtr _collider;
};
}

#endif //HINAPE_SOLVER_H
