#ifndef HINAPE_POINT_PARTICLE_EMITTER_H
#define HINAPE_POINT_PARTICLE_EMITTER_H

#include "particle_emitter.h"

namespace HinaPE
{
class PointParticleEmitter3 final : public ParticleEmitter3
{
public:
	void emit(std::vector<mVector3> &positions, std::vector<mVector3> &velocities) override;

public:
	struct Opt
	{
		mVector3 origin;
		mVector3 direction;
		real speed;
		real spread_angle;

		size_t max_new_particles_per_sec = 1;
		size_t max_particles = 100000;
	} _opt;
	void INSPECT() final;
	PointParticleEmitter3();

private:
	std::mt19937 _rng;
};
} // namespace HinaPE

#endif //HINAPE_POINT_PARTICLE_EMITTER_H
