#include "point_particle_emitter.h"

HinaPE::PointParticleEmitter3::PointParticleEmitter3() : _rng(random()) {}
void HinaPE::PointParticleEmitter3::emit(std::vector<mVector3> &positions, std::vector<mVector3> &velocities)
{
	for (int i = 0; i < _opt.max_new_particles_per_sec; ++i)
	{
		
	}
}
void HinaPE::PointParticleEmitter3::INSPECT()
{
	ParticleEmitter3::INSPECT();
	INSPECT_VEC3(_opt.origin, "origin");
	INSPECT_VEC3(_opt.direction, "direction");
	INSPECT_REAL(_opt.speed, "speed");
	INSPECT_REAL(_opt.spread_angle, "spread angle");
}
