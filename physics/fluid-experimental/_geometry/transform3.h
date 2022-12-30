#ifndef HINAPE_TRANSFORM3_H
#define HINAPE_TRANSFORM3_H

#include "math_api.h"

namespace HinaPE
{
class Transform3
{
public:
	inline auto to_local(const mBBox &bbox_in_world) const -> mBBox
	{
		mBBox bbox_in_local;
		for (int i = 0; i < 8; ++i)
		{
			auto corner_in_local = to_local(bbox_in_world.corner(i));
			bbox_in_local.lowerCorner = min(bbox_in_local.lowerCorner, corner_in_local);
			bbox_in_local.upperCorner = max(bbox_in_local.upperCorner, corner_in_local);
		}
		return bbox_in_local;
	}
	inline auto to_world(const mBBox &bbox_in_local) const -> mBBox
	{
		mBBox bbox_in_world;
		for (int i = 0; i < 8; ++i)
		{
			auto corner_in_world = to_world(bbox_in_local.corner(i));
			bbox_in_world.lowerCorner = min(bbox_in_world.lowerCorner, corner_in_world);
			bbox_in_world.upperCorner = max(bbox_in_world.upperCorner, corner_in_world);
		}
		return bbox_in_world;
	}
	inline auto to_local(const mRay3 &ray) const -> mRay3 { return {to_local(ray.origin), to_local(ray.direction)}; }
	inline auto to_world(const mRay3 &ray) const -> mRay3 { return {to_world(ray.origin), to_world(ray.direction)}; }
	inline auto to_local(const mVector3 &point_in_world) const -> mVector3 { return _inverse_orientation_mat3 * (point_in_world - _translation); }
	inline auto to_world(const mVector3 &point_in_local) const -> mVector3 { return _orientation_mat3 * point_in_local + _translation; }
	inline auto to_local_direction(const mVector3 &dir_in_world) const -> mVector3 { return _inverse_orientation_mat3 * dir_in_world; }
	inline auto to_world_direction(const mVector3 &dir_in_local) const -> mVector3 { return _orientation_mat3 * dir_in_local; }

public:
	mVector3 _translation;
	mQuaternion _orientation;
	mMatrix3x3 _orientation_mat3;
	mMatrix3x3 _inverse_orientation_mat3;
};
}

#endif //HINAPE_TRANSFORM3_H
