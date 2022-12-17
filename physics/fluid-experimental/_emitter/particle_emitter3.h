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
//	std::function<void(ParticleSystemData3Ptr)>

public:
	struct Opt
	{
		bool enable = false;
	} _opt;
	inline auto target() const -> ParticleSystemData3Ptr { return _particles; }
	inline void set_enable(bool enable) { _opt.enable = enable; }

public:
	explicit ParticleEmitter3(ParticleSystemData3Ptr ptr) : _particles(std::move(ptr)) {};

protected:
	virtual void on_update(real current_time, real dt) = 0;

protected:
	ParticleSystemData3Ptr _particles;
};
using ParticleEmitter3Ptr = std::shared_ptr<ParticleEmitter3>;
}

#endif //HINAPE_PARTICLE_EMITTER3_H
