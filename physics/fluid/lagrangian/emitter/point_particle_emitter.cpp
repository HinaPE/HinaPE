#include "point_particle_emitter.h"

HinaPE::PointParticleEmitter3::PointParticleEmitter3() : _rng(rand()) {}
void HinaPE::PointParticleEmitter3::emit(std::vector<mVector3> &positions, std::vector<mVector3> &velocities)
{
	if (!ParticleEmitter3::_opt.enable || _opt.remaining_particles <= 0)
		return;

	auto rdm = [&]() -> real
	{
		std::uniform_real_distribution<> d(0.0, 1.0);
		return d(_rng);
	};

	for (int i = 0; i < _opt.particles_at_once; ++i)
	{
		auto new_dir = Math::uniform_sample_cone(rdm(), rdm(), _opt.direction, _opt.spread_angle);
		positions.push_back(_opt.origin);
		velocities.push_back(_opt.speed * new_dir);
	}
	_opt.remaining_particles -= _opt.particles_at_once;
}
void HinaPE::PointParticleEmitter3::INSPECT()
{
	ParticleEmitter3::INSPECT();
	INSPECT_VEC3(_opt.origin, "origin");
	INSPECT_VEC3(_opt.direction, "direction");
	INSPECT_REAL(_opt.speed, "speed");
	INSPECT_REAL(_opt.spread_angle, "spread angle");
}
