#ifndef HINAPE_PARTICLE_EMITTER_H
#define HINAPE_PARTICLE_EMITTER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "renderer3D/objects/arrow.h"

namespace HinaPE
{
class ParticleEmitter3 : public Kasumi::ArrowObject
{
public:
	virtual void emit(std::vector<mVector3> &positions, std::vector<mVector3> &velocities) = 0;

public:
	struct Opt
	{
		bool enable = true;

		mVector3 origin = mVector3::Zero();
		mVector3 direction = mVector3::UnitY();
		real speed = 20;
		real spread_angle = 90;

		size_t particles_at_once = 100;
		size_t remaining_particles = 10000;
	} _opt;
	void INSPECT() override;
	ParticleEmitter3();
};
using ParticleEmitter3Ptr = std::shared_ptr<ParticleEmitter3>;
} // namespace HinaPE

#endif //HINAPE_PARTICLE_EMITTER_H
