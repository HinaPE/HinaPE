#include "volume_particle_emitter3.h"

HinaPE::Fluid::VolumeParticleEmitter3::VolumeParticleEmitter3(ParticleSystemData3Ptr ptr, ImplicitSurface3Ptr surface, const Opt &opt) : ParticleEmitter3(std::move(ptr)), _implicit_surface(std::move(surface)), _opt(opt), _rng(rand()) {}
void HinaPE::Fluid::VolumeParticleEmitter3::on_update(real current_time, real dt)
{
	if (!_opt.enable)
		return;

	_implicit_surface->updateQueryEngine(); // TODO: understand this

	BoundingBox3D region = _bounds;
	if (_implicit_surface->isBounded()) // TODO: understand this
	{
		auto surface_bbox = _implicit_surface->boundingBox();
		region.lowerCorner = max(region.lowerCorner, surface_bbox.lowerCorner);
		region.upperCorner = min(region.upperCorner, surface_bbox.upperCorner);
	}

	const real j = _opt.jitter;
	const real max_jitter_dist = static_cast<real>(0.5) * j * _opt.spacing;
	size_t new_particle_num = 0;

	if (_opt.allow_overlapping || _opt.is_one_shut)
	{
	}

	if (_opt.is_one_shut)
		_opt.enable = false;
}
