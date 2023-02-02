#include "geom/surface3.h"

// ============================== Box ==============================
auto HinaPE::Geom::Box3::_intersects_local(const mRay3 &ray) const -> bool
{
	return false;
}
auto HinaPE::Geom::Box3::_bounding_box_local() const -> mBBox3
{
	return {};
}
auto HinaPE::Geom::Box3::_closest_point_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}
auto HinaPE::Geom::Box3::_closest_intersection_local(const mRay3 &ray) const -> HinaPE::Geom::SurfaceRayIntersection3
{
	return {};
}
auto HinaPE::Geom::Box3::_closest_distance_local(const mVector3 &other_point) const -> real
{
	return 0;
}
auto HinaPE::Geom::Box3::_closest_normal_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}

// ============================== Sphere ==============================
auto HinaPE::Geom::Sphere3::_intersects_local(const mRay3 &ray) const -> bool
{
	return false;
}
auto HinaPE::Geom::Sphere3::_bounding_box_local() const -> mBBox3
{
	return {};
}
auto HinaPE::Geom::Sphere3::_closest_point_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}
auto HinaPE::Geom::Sphere3::_closest_intersection_local(const mRay3 &ray) const -> HinaPE::Geom::SurfaceRayIntersection3
{
	return {};
}
auto HinaPE::Geom::Sphere3::_closest_distance_local(const mVector3 &other_point) const -> real
{
	return 0;
}
auto HinaPE::Geom::Sphere3::_closest_normal_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}

// ============================== Plane ==============================
auto HinaPE::Geom::Plane3::_intersects_local(const mRay3 &ray) const -> bool
{
	return false;
}
auto HinaPE::Geom::Plane3::_bounding_box_local() const -> mBBox3
{
	return {};
}
auto HinaPE::Geom::Plane3::_closest_point_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}
auto HinaPE::Geom::Plane3::_closest_intersection_local(const mRay3 &ray) const -> HinaPE::Geom::SurfaceRayIntersection3
{
	return {};
}
auto HinaPE::Geom::Plane3::_closest_distance_local(const mVector3 &other_point) const -> real
{
	return 0;
}
auto HinaPE::Geom::Plane3::_closest_normal_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}


// ============================== Cylinder3 ==============================
auto HinaPE::Geom::Cylinder3::_intersects_local(const mRay3 &ray) const -> bool
{
	return false;
}
auto HinaPE::Geom::Cylinder3::_bounding_box_local() const -> mBBox3
{
	return {};
}
auto HinaPE::Geom::Cylinder3::_closest_point_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}
auto HinaPE::Geom::Cylinder3::_closest_intersection_local(const mRay3 &ray) const -> HinaPE::Geom::SurfaceRayIntersection3
{
	return {};
}
auto HinaPE::Geom::Cylinder3::_closest_distance_local(const mVector3 &other_point) const -> real
{
	return 0;
}
auto HinaPE::Geom::Cylinder3::_closest_normal_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}


// ============================== SurfaceToImplicit3 ==============================
HinaPE::Geom::SurfaceToImplicit3::SurfaceToImplicit3(const std::shared_ptr<Surface3> &surface) : _surface(surface)
{
}
auto HinaPE::Geom::SurfaceToImplicit3::_intersects_local(const mRay3 &ray) const -> bool
{
	return false;
}
auto HinaPE::Geom::SurfaceToImplicit3::_bounding_box_local() const -> mBBox3
{
	return {};
}
auto HinaPE::Geom::SurfaceToImplicit3::_closest_point_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}
auto HinaPE::Geom::SurfaceToImplicit3::_closest_intersection_local(const mRay3 &ray) const -> HinaPE::Geom::SurfaceRayIntersection3
{
	return {};
}
auto HinaPE::Geom::SurfaceToImplicit3::_closest_normal_local(const mVector3 &other_point) const -> mVector3
{
	return {};
}
auto HinaPE::Geom::SurfaceToImplicit3::_signed_distance_local(const mVector3 &other_point) const -> real
{
	return 0;
}
