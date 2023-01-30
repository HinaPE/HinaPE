//#ifndef HINAPE_TRANSFORM3_H
//#define HINAPE_TRANSFORM3_H
//
//#include "base/base.h"
////#include "ray.h"
//
//namespace Hina
//{
//class Transform3
//{
//public:
//	inline auto to_local(const Base::mBBox3 &bbox_in_world) const -> Base::mBBox3
//	{
//		Base::mBBox3 bbox_in_local;
//		for (int i = 0; i < 8; ++i)
//		{
//			auto corner_in_local = to_local(bbox_in_world.corner(i));
//			bbox_in_local.lower_corner = min(bbox_in_local.lower_corner, corner_in_local);
//			bbox_in_local.upper_corner = max(bbox_in_local.upper_corner, corner_in_local);
//		}
//		return bbox_in_local;
//	}
//	inline auto to_world(const Base::mBBox3 &bbox_in_local) const -> Base::mBBox3
//	{
//		Base::mBBox3 bbox_in_world;
//		for (int i = 0; i < 8; ++i)
//		{
//			auto corner_in_world = to_world(bbox_in_local.corner(i));
//			bbox_in_world.lower_corner = min(bbox_in_world.lower_corner, corner_in_world);
//			bbox_in_world.upper_corner = max(bbox_in_world.upper_corner, corner_in_world);
//		}
//		return bbox_in_world;
//	}
//	inline auto to_local(const Ray3 &ray) const -> Ray3 { return {to_local(ray._origin), to_local(ray._direction)}; }
//	inline auto to_world(const Ray3 &ray) const -> Ray3 { return {to_world(ray._origin), to_world(ray._direction)}; }
//	inline auto to_local(const Base::mVector3 &point_in_world) const -> Base::mVector3 { return _inverse_orientation_mat3 * (point_in_world - _translation); }
//	inline auto to_world(const Base::mVector3 &point_in_local) const -> Base::mVector3 { return _orientation_mat3 * point_in_local + _translation; }
//	inline auto to_local_direction(const Base::mVector3 &dir_in_world) const -> Base::mVector3 { return _inverse_orientation_mat3 * dir_in_world; }
//	inline auto to_world_direction(const Base::mVector3 &dir_in_local) const -> Base::mVector3 { return _orientation_mat3 * dir_in_local; }
//
//public:
//	Base::mVector3 _translation;
//	Base::mQuaternion _orientation;
//	Base::mMatrix3x3 _orientation_mat3;
//	Base::mMatrix3x3 _inverse_orientation_mat3;
//};
//}
//
//#endif //HINAPE_TRANSFORM3_H
