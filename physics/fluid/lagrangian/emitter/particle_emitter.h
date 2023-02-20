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
		bool enable = false;

		mVector3 position = mVector3::Zero();
		mVector3 direction = mVector3::UnitZ();

		size_t particles_at_once = 1;
		size_t remaining_particles = 100;
	} _opt;
	void INSPECT() override;
	ParticleEmitter3();
};
using ParticleEmitter3Ptr = std::shared_ptr<ParticleEmitter3>;
} // namespace HinaPE

#endif //HINAPE_PARTICLE_EMITTER_H
