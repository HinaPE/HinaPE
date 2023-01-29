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
	GridFluidSolver();

protected:
	void on_physics_init() final;
	void on_physics_update(float time_step) final;

private:
	GridSystemData3Ptr _grids;
	GridEmitter3Ptr _emitter;
};
}

#endif //HINAPE_SOLVER_H
