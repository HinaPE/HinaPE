#include "surface3.h"

#include "pch.h"

#include <utility>

using namespace HinaPE;

Surface3::Surface3(Transform3 transform_, bool isNormalFlipped_) : transform(std::move(transform_)), isNormalFlipped(isNormalFlipped_) {}

Surface3::Surface3(const Surface3 &other) = default;

Surface3::~Surface3() = default;

auto Surface3::closestPoint(const Vector3D &otherPoint) const -> Vector3D
{
    return transform.toWorld(closestPointLocal(transform.toLocal(otherPoint)));
}

auto Surface3::boundingBox() const -> BoundingBox3D
{
    return transform.toWorld(boundingBoxLocal());
}

auto Surface3::intersects(const Ray3D &ray) const -> bool
{
    return intersectsLocal(transform.toLocal(ray));
}

auto Surface3::closestDistance(const Vector3D &otherPoint) const -> double
{
    return closestDistanceLocal(transform.toLocal(otherPoint));
}

auto Surface3::closestIntersection(const Ray3D &ray) const -> SurfaceRayIntersection3
{
    auto result = closestIntersectionLocal(transform.toLocal(ray));
    result.point = transform.toWorld(result.point);
    result.normal = transform.toWorldDirection(result.normal);
    result.normal *= (isNormalFlipped) ? -1.0 : 1.0;
    return result;
}

auto Surface3::closestNormal(const Vector3D &otherPoint) const -> Vector3D
{
    auto result = transform.toWorldDirection(closestNormalLocal(transform.toLocal(otherPoint)));
    result *= (isNormalFlipped) ? -1.0 : 1.0;
    return result;
}

auto Surface3::intersectsLocal(const Ray3D &rayLocal) const -> bool
{
    auto result = closestIntersectionLocal(rayLocal);
    return result.isIntersecting;
}

void Surface3::updateQueryEngine()
{
    // Do nothing
}

auto Surface3::isBounded() const -> bool { return true; }

auto Surface3::isValidGeometry() const -> bool { return true; }

auto Surface3::isInside(const Vector3D &otherPoint) const -> bool
{
    return isNormalFlipped == !isInsideLocal(transform.toLocal(otherPoint));
}

auto Surface3::closestDistanceLocal(const Vector3D &otherPointLocal) const -> double
{
    return otherPointLocal.distanceTo(closestPointLocal(otherPointLocal));
}

auto Surface3::isInsideLocal(const Vector3D &otherPointLocal) const -> bool
{
    Vector3D cpLocal = closestPointLocal(otherPointLocal);
    Vector3D normalLocal = closestNormalLocal(otherPointLocal);
    return (otherPointLocal - cpLocal).dot(normalLocal) < 0.0;
}
