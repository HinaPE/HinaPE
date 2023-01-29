#ifndef HINAPE_SOLVER_H
#define HINAPE_SOLVER_H

#include "animation/physics_animation.h"
#include "data.h"

namespace Hina
{
class GridFluidSolver : public PhysicsAnimation
{
protected:
	void on_physics_init() final;
	void on_physics_update(float time_step) final;
};
}

#endif //HINAPE_SOLVER_H
