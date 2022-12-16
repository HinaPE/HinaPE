#ifndef HINAPE_VOLUME_PARTICLE_EMITTER3_H
#define HINAPE_VOLUME_PARTICLE_EMITTER3_H

#include "particle_emitter3.h"
#include "geometry/implicit_surface3.h"
#include "_point_generator/point_generator3.h"

#include <random>

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
	std::mt19937 _rng;
	ImplicitSurface3Ptr _implicit_surface;
	BoundingBox3D _bounds;
};
}

#endif //HINAPE_VOLUME_PARTICLE_EMITTER3_H
