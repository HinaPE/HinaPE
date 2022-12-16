#ifndef HINAPE_VOLUME_PARTICLE_EMITTER3_H
#define HINAPE_VOLUME_PARTICLE_EMITTER3_H

#include "particle_emitter3.h"
#include "geometry/implicit_surface3.h"

namespace HinaPE::Fluid
{
class VolumeParticleEmitter3 final : public ParticleEmitter3
{
public:
	struct Opt
	{
		bool enable = false;

		float spacing;

		bool is_one_shut = false;
		bool allow_overlapping = false;
		float jitter = 0.f;

		mVector3 initial_velocity;
		mVector3 linear_velocity;
		mVector3 angular_velocity;

		size_t current_particle_num = 0;
		size_t max_particle_num = 1e6;
	} _opt;

public:
	VolumeParticleEmitter3(ParticleSystemData3Ptr ptr, ImplicitSurface3Ptr surface, const Opt &opt);

private:
	void on_update(real current_time, real dt) final;

private:
	ImplicitSurface3Ptr _implicit_surface;
};
}

#endif //HINAPE_VOLUME_PARTICLE_EMITTER3_H
