#include "volume_particle_emitter3.h"

#include "samplers.h"

#include <utility>
#include <stdexcept>

HinaPE::Fluid::VolumeParticleEmitter3::VolumeParticleEmitter3(ParticleSystemData3Ptr ptr, ImplicitSurface3Ptr surface, const mBBox &max_region, Opt opt) : ParticleEmitter3(std::move(ptr)), _implicit_surface(std::move(surface)), _bounds(max_region), _opt(std::move(opt)), _rng(rand())
{
	_point_generator = std::make_shared<BccLatticePointGenerator>();
}
void HinaPE::Fluid::VolumeParticleEmitter3::on_update(real current_time, real dt)
{
	if (!_opt.enable)
		return;

	_implicit_surface->updateQueryEngine(); // TODO: understand this

	mBBox region = _bounds;
	if (_implicit_surface->isBounded()) // TODO: understand this
	{
		// the return type now is double, but we use real, so we need to transform it now
		// in the future, we may unify them
		auto surface_bbox = _implicit_surface->boundingBox();

		mVector3 rl = {region.lowerCorner.x, region.lowerCorner.y, region.lowerCorner.z};
		mVector3 ru = {region.upperCorner.x, region.upperCorner.y, region.upperCorner.z};
		mVector3 sl = {surface_bbox.lowerCorner.x, surface_bbox.lowerCorner.y, surface_bbox.lowerCorner.z};
		mVector3 su = {surface_bbox.upperCorner.x, surface_bbox.upperCorner.y, surface_bbox.upperCorner.z};

		region.lowerCorner = max(rl, sl);
		region.upperCorner = min(ru, su);
	}

	const real j = _opt.jitter;
	const real max_jitter_dist = static_cast<real>(0.5) * j * _opt.spacing;
	size_t new_particle_num = 0;

	Array1<mVector3> newPositions;
	Array1<mVector3> newVelocities;

	if (_opt.allow_overlapping || _opt.is_one_shut)
	{
		_point_generator->forEachPoint(region, _opt.spacing, [&](const mVector3 &point)
		{
			std::uniform_real_distribution<> d(0.f, 1.f);

			Vector3D random_dirD = uniformSampleSphere(d(_rng), d(_rng)); // TODO: remove this

			mVector3 random_dir = {random_dirD.x, random_dirD.y, random_dirD.z};

			mVector3 offset = max_jitter_dist * random_dir;
			mVector3 candidate = point + offset;

			Vector3D candidateD = {candidate.x, candidate.y, candidate.z}; // TODO: remove this

			if (_implicit_surface->signedDistance(candidateD) <= 0.0)
			{
				if (_opt.current_particle_num < _opt.max_particle_num)
				{
					newPositions.append(candidate);
					++_opt.current_particle_num;
					++new_particle_num;
				} else
					return false;
			}

			return true;
		});
	} else
	{
		// TODO: support continuous emission
		throw std::runtime_error("not implemented");
	}

	newVelocities.resize(newPositions.size());
	newVelocities.parallelForEachIndex(
			[&](size_t i)
			{
				Vector3D point = {newPositions[i].x, newPositions[i].y, newPositions[i].z}; // TODO: remove this
				Vector3D rD = point - _implicit_surface->transform.translation();
				mVector3 r = {rD.x, rD.y, rD.z}; // TODO: remove this
				newVelocities[i] = _opt.initial_velocity + _opt.linear_velocity + _opt.angular_velocity.cross(r);
			});

	if (_opt.is_one_shut)
		_opt.enable = false;
}
