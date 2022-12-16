#ifndef HINAPE_PARTICLE_EMITTER3_H
#define HINAPE_PARTICLE_EMITTER3_H

#include "_solver/particle/particle_system_data3.h"
#include <memory>
#include <utility>

namespace HinaPE::Fluid
{
class ParticleEmitter3
{
public:
	void update(real current_time, real dt);

public:
	struct Opt {} _opt;

public:
	explicit ParticleEmitter3(ParticleSystemData3Ptr ptr) : _particles(std::move(ptr)) {};

protected:
	virtual void on_update(real current_time, real dt) = 0;

private:
	ParticleSystemData3Ptr _particles;
};
using ParticleEmitter3Ptr = std::shared_ptr<ParticleEmitter3>;
}

#endif //HINAPE_PARTICLE_EMITTER3_H
