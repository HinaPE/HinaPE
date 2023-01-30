#ifndef HINAPE_SOLVER_H
#define HINAPE_SOLVER_H

#include "animation/physics_animation.h"
#include "data.h"
#include "emitter.h"

namespace Hina
{
class GridFluidSolver : public PhysicsAnimation
{
public:
	struct Opt
	{
		// Env
		Base::mVector3 gravity = Base::mVector3(0, -9.8, 0);

		Base::real viscosity = 0.0;
		Base::real max_cfl = 5.0;
	} _opt;

public:
	GridFluidSolver(const Base::Size3 &resolution, const Base::mVector3 &grid_spacing, const Base::mVector3 &origin);

protected:
	void on_physics_init() final;
	void on_physics_update(float time_step) final;

private:
	GridSystemData3Ptr _grids;
	GridEmitter3Ptr _emitter;
};
}

#endif //HINAPE_SOLVER_H
