#ifndef HINAPE_FLUID_PARTICLE_SYSTEM_SOLVER3_H
#define HINAPE_FLUID_PARTICLE_SYSTEM_SOLVER3_H

#include "_animation/physics_animation.h"
#include "particle_system_data3.h"
#include "_emitter/particle_emitter3.h"
#include "geometry/collider3.h"

namespace HinaPE::Fluid
{
class ParticleSystemSolver3 : public PhysicsAnimation
{
public:
	inline auto particle_positions() const -> ConstArrayAccessor1<mVector3> { return _particle_system_data->positions(); }

public:
	struct Opt : public PhysicsAnimation::Opt
	{
		mVector3 gravity = {0, -9.8f, 0};
		float drag_coefficient = 1e-4;
		float restitution_coefficient = 0.0f;
	} _opt;

protected:
	void on_init_physics() override; // allow to override again
	void on_update_physics(float time_step) override; // allow to override again
	virtual void on_begin_particle_system_update(real dt) {}; // to be overridden
	virtual void accumulate_force(float dt);
	virtual void on_end_particle_system_update(real dt) {}; // to be overridden
	void resolve_collision();
	void resolve_collision(ArrayAccessor1<mVector3> positions, ArrayAccessor1<mVector3> velocities);

private:
	void begin_particle_system_update(real dt);
	void end_particle_system_update(real dt);
	void time_integration(real dt);

private:
	ParticleSystemData3Ptr _particle_system_data;
	ParticleEmitter3Ptr _emitter;
	Collider3Ptr _collider;
	Array1<mVector3> _new_positions; // cache for next phase data
	Array1<mVector3> _new_velocities; // cache for next phase data
};
using ParticleSystemSolver3Ptr = std::shared_ptr<ParticleSystemSolver3>;
}

#endif //HINAPE_FLUID_PARTICLE_SYSTEM_SOLVER3_H
