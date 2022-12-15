#ifndef HINAPE_FLUID_PARTICLE_SYSTEM_SOLVER3_H
#define HINAPE_FLUID_PARTICLE_SYSTEM_SOLVER3_H

#include "../../animation/physics_animation.h"

namespace HinaPE
{
class ParticleSystemSolver3 : public PhysicsAnimation
{
public:
	struct Opt
	{
		float drag_coefficient = 1e-4;
		float restitution_coefficient = 0.0f;
	} _opt;

protected:
	void on_init_physics() override;
	void on_update_physics(float time_step) final;

private:
};
using ParticleSystemSolver3Ptr = std::shared_ptr<ParticleSystemSolver3>;
}

#endif //HINAPE_FLUID_PARTICLE_SYSTEM_SOLVER3_H
