#include "particle_emitter.h"

HinaPE::PointParticleEmitter3::PointParticleEmitter3() : _rng(0) {}
void HinaPE::PointParticleEmitter3::emit(std::vector<mVector3> *positions, std::vector<mVector3> *velocities)
{
	if (!_opt.enable || _opt.remaining_particles <= 0)
		return;

	auto rdm = [&]() -> real
	{
		std::uniform_real_distribution<> d(0.0, 1.0);
		return d(_rng);
	};

	for (int i = 0; i < _opt.particles_at_once; ++i)
	{
		auto new_dir = Math::uniform_sample_cone(rdm(), rdm(), _direction, _opt.spread_angle);
		positions->push_back(_origin);
		velocities->push_back(_opt.speed * new_dir);
	}
	_opt.remaining_particles -= _opt.particles_at_once;
}

HinaPE::VolumeParticleEmitter3::VolumeParticleEmitter3() : _rng(0) {}
void HinaPE::VolumeParticleEmitter3::emit(std::vector<mVector3> *positions, std::vector<mVector3> *velocities)
{
	if (_opt.one_shot && _opt.shot)
		return;

	auto width = 0.7, height = 0.4, depth = 0.7;
	mVector3 start_point = {-width / 2, 0, -depth / 2};
	auto rows = static_cast<size_t>(width / _opt.spacing);
	auto cols = static_cast<size_t>(height / _opt.spacing);
	auto layers = static_cast<size_t>(depth / _opt.spacing);

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			for (int k = 0; k < layers; ++k)
			{
				positions->push_back(start_point + mVector3(i * _opt.spacing, j * _opt.spacing, k * _opt.spacing));
				velocities->push_back(mVector3(0, 0, 0));
			}
		}
	}

	_opt.shot = true;
}
