#ifndef HINAPE_PARTICLE_EMITTER_H
#define HINAPE_PARTICLE_EMITTER_H

#include "backends/api.h"

class ParticleEmitter3 : public Kasumi::INSPECTOR
{
public:
	virtual void emit(std::vector<mVector3> &positions, std::vector<mVector3> &velocities) = 0;

public:
	struct Opt
	{
		bool enable = false;
	} _opt;
	void INSPECT() override;
	ParticleEmitter3();
};


#endif //HINAPE_PARTICLE_EMITTER_H
