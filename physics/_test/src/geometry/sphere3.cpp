//#include "sphere3.h"
//
//auto Hina::Sphere3::intersects_local(const mRay3 &ray) const -> bool
//{
//	mVector3 r = ray.origin - _center;
//	real b = ray.direction.dot(r);
//	real c = r.lengthSquared() - square(_radius);
//	real d = b * b - c;
//
//	if (d > 0.)
//	{
//		d = std::sqrt(d);
//		real tMin = -b - d;
//		real tMax = -b + d;
//
//		if (tMin < 0.0)
//			tMin = tMax;
//
//		if (tMin >= 0.0)
//			return true;
//	}
//
//	return false;
//}
//auto Hina::Sphere3::bounding_box_local() const -> Base::mBBox
//{
//	Base::mVector3 r(_radius, _radius, _radius);
//	return {_center - r, _center + r};
//}
//auto Hina::Sphere3::closest_point_local(const Base::mVector3 &other_point) const -> Base::mVector3
//{
//	return _radius * closest_normal_local(other_point) + _center;
//}
//auto Hina::Sphere3::closest_intersection_local(const mRay3 &ray) const -> Hina::SurfaceRayIntersection3
//{
//	SurfaceRayIntersection3 intersection;
//	Base::mVector3 r = ray.origin - _center;
//	Base::real b = ray.direction.dot(r);
//	Base::real c = r.lengthSquared() - square(_radius);
//	Base::real d = b * b - c;
//
//	if (d > 0.)
//	{
//		d = std::sqrt(d);
//		real tMin = -b - d;
//		real tMax = -b + d;
//
//		if (tMin < 0.0)
//			tMin = tMax;
//
//		if (tMin < 0.0)
//			intersection.is_intersecting = false;
//		else
//		{
//			intersection.is_intersecting = true;
//			intersection.distance = tMin;
//			intersection.point = ray.origin + tMin * ray.direction;
//			intersection.normal = (intersection.point - _center).normalized();
//		}
//	} else
//		intersection.is_intersecting = false;
//
//	return intersection;
//}
//auto Hina::Sphere3::closest_distance_local(const Base::mVector3 &other_point) const -> Base::real
//{
//	return std::fabs(_center.distanceTo(other_point) - _radius);
//}
//auto Hina::Sphere3::closest_normal_local(const Base::mVector3 &other_point) const -> Base::mVector3
//{
//	if (_center.isSimilar(other_point))
//		return {1, 0, 0};
//	else
//		return (other_point - _center).normalized();
//}
