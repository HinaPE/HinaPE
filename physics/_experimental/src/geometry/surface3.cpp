#include "surface3.h"
// ============================== Box ==============================
auto Hina::Box3::_intersects_local(const Hina::mRay3 &ray) const -> bool
{
	return false;
}
auto Hina::Box3::_bounding_box_local() const -> Hina::mBBox3
{
	return {};
}
auto Hina::Box3::_closest_point_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}
auto Hina::Box3::_closest_intersection_local(const Hina::mRay3 &ray) const -> Hina::SurfaceRayIntersection3
{
	return {};
}
auto Hina::Box3::_closest_distance_local(const Hina::mVector3 &other_point) const -> Hina::real
{
	return 0;
}
auto Hina::Box3::_closest_normal_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}

// ============================== Sphere ==============================
auto Hina::Sphere3::_intersects_local(const Hina::mRay3 &ray) const -> bool
{
	return false;
}
auto Hina::Sphere3::_bounding_box_local() const -> Hina::mBBox3
{
	return {};
}
auto Hina::Sphere3::_closest_point_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}
auto Hina::Sphere3::_closest_intersection_local(const Hina::mRay3 &ray) const -> Hina::SurfaceRayIntersection3
{
	return {};
}
auto Hina::Sphere3::_closest_distance_local(const Hina::mVector3 &other_point) const -> Hina::real
{
	return 0;
}
auto Hina::Sphere3::_closest_normal_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}

// ============================== Plane ==============================
auto Hina::Plane3::_intersects_local(const Hina::mRay3 &ray) const -> bool
{
	return false;
}
auto Hina::Plane3::_bounding_box_local() const -> Hina::mBBox3
{
	return {};
}
auto Hina::Plane3::_closest_point_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}
auto Hina::Plane3::_closest_intersection_local(const Hina::mRay3 &ray) const -> Hina::SurfaceRayIntersection3
{
	return {};
}
auto Hina::Plane3::_closest_distance_local(const Hina::mVector3 &other_point) const -> Hina::real
{
	return 0;
}
auto Hina::Plane3::_closest_normal_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}


// ============================== Plane ==============================
auto Hina::Cylinder3::_intersects_local(const Hina::mRay3 &ray) const -> bool
{
	return false;
}
auto Hina::Cylinder3::_bounding_box_local() const -> Hina::mBBox3
{
	return {};
}
auto Hina::Cylinder3::_closest_point_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}
auto Hina::Cylinder3::_closest_intersection_local(const Hina::mRay3 &ray) const -> Hina::SurfaceRayIntersection3
{
	return {};
}
auto Hina::Cylinder3::_closest_distance_local(const Hina::mVector3 &other_point) const -> Hina::real
{
	return 0;
}
auto Hina::Cylinder3::_closest_normal_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}
auto Hina::SurfaceToImplicit3::_intersects_local(const Hina::mRay3 &ray) const -> bool
{
	return false;
}
auto Hina::SurfaceToImplicit3::_bounding_box_local() const -> Hina::mBBox3
{
	return {};
}
auto Hina::SurfaceToImplicit3::_closest_point_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}
auto Hina::SurfaceToImplicit3::_closest_intersection_local(const Hina::mRay3 &ray) const -> Hina::SurfaceRayIntersection3
{
	return {};
}
auto Hina::SurfaceToImplicit3::_closest_normal_local(const Hina::mVector3 &other_point) const -> Hina::mVector3
{
	return {};
}
auto Hina::SurfaceToImplicit3::_signed_distance_local(const Hina::mVector3 &other_point) const -> Hina::real
{
	return 0;
}
