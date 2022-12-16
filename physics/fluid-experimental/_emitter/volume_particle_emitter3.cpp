#include "volume_particle_emitter3.h"

HinaPE::Fluid::VolumeParticleEmitter3::VolumeParticleEmitter3(ParticleSystemData3Ptr ptr, ImplicitSurface3Ptr surface, const Opt &opt) : ParticleEmitter3(std::move(ptr)), _implicit_surface(std::move(surface)), _opt(opt) {}
void HinaPE::Fluid::VolumeParticleEmitter3::on_update(real current_time, real dt)
{
	if (!_opt.enable)
		return;


}
