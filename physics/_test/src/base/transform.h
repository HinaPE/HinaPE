#ifndef HINAPE_TRANSFORM3_H
#define HINAPE_TRANSFORM3_H

#ifdef HINAPE_EIGEN
#include "Eigen/Eigen"
#endif
#include <type_traits>

#include "vector.h"
#include "quaternion.h"
#include "matrix.h"
#include "bbox.h"

namespace Hina::Base
{
template<typename T>
class Transform3
{
public:
	inline auto to_local(const BoundingBox3<T> &bbox_in_world) const -> BoundingBox3<T>
	{
		BoundingBox3<T> bbox_in_local;
		for (int i = 0; i < 8; ++i)
		{
			auto corner_in_local = to_local(bbox_in_world.corner(i));
			bbox_in_local._lower_corner = min(bbox_in_local._lower_corner, corner_in_local);
			bbox_in_local._upper_corner = max(bbox_in_local._upper_corner, corner_in_local);
		}
		return bbox_in_local;
	}
	inline auto to_world(const BoundingBox3<T> &bbox_in_local) const -> BoundingBox3<T>
	{
		BoundingBox3<T> bbox_in_world;
		for (int i = 0; i < 8; ++i)
		{
			auto corner_in_world = to_world(bbox_in_local.corner(i));
			bbox_in_world._lower_corner = min(bbox_in_world._lower_corner, corner_in_world);
			bbox_in_world._upper_corner = max(bbox_in_world._upper_corner, corner_in_world);
		}
		return bbox_in_world;
	}
//	inline auto to_local(const Ray3 &ray) const -> Ray3 { return {to_local(ray._origin), to_local(ray._direction)}; }
//	inline auto to_world(const Ray3 &ray) const -> Ray3 { return {to_world(ray._origin), to_world(ray._direction)}; }
	inline auto to_local(const Vector3<T> &point_in_world) const -> Vector3<T> { return _inverse_orientation_mat3 * (point_in_world - _translation); }
	inline auto to_world(const Vector3<T> &point_in_local) const -> Vector3<T> { return _orientation_mat3 * point_in_local + _translation; }
//	inline auto to_local_direction(const mVector3 &dir_in_world) const -> mVector3 { return _inverse_orientation_mat3 * dir_in_world; }
//	inline auto to_world_direction(const mVector3 &dir_in_local) const -> mVector3 { return _orientation_mat3 * dir_in_local; }

private:
#ifdef HINAPE_EIGEN
	Vector3<T> _translation;
	Quaternion<T> _orientation;
	Matrix3x3<T> _orientation_mat3;
	Matrix3x3<T> _inverse_orientation_mat3;
//	constexpr explicit Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_);
#endif
};

#ifdef HINAPE_EIGEN
#include "base/impl/transform_impl_eigen.h"
#else
#endif
}
#endif //HINAPE_TRANSFORM3_H
