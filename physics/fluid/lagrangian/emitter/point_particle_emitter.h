#ifndef HINAPE_POINT_PARTICLE_EMITTER_H
#define HINAPE_POINT_PARTICLE_EMITTER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

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
	} _opt;
	void INSPECT() final;
	PointParticleEmitter3();

private:
	std::mt19937 _rng;
};
} // namespace HinaPE

#endif //HINAPE_POINT_PARTICLE_EMITTER_H
