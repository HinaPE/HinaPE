#include "surface2.h"

#include "pch.h"

#include <utility>

using namespace HinaPE::FluidEngine;

Surface2::Surface2(Transform2 transform_, bool isNormalFlipped_) : transform(std::move(transform_)), isNormalFlipped(isNormalFlipped_) {}

Surface2::Surface2(const Surface2 &other) = default;

Surface2::~Surface2() = default;

auto Surface2::closestPoint(const Vector2D &otherPoint) const -> Vector2D
{
    return transform.toWorld(closestPointLocal(transform.toLocal(otherPoint)));
}

auto Surface2::boundingBox() const -> BoundingBox2D
{
    return transform.toWorld(boundingBoxLocal());
}

auto Surface2::intersects(const Ray2D &ray) const -> bool
{
    return intersectsLocal(transform.toLocal(ray));
}

auto Surface2::closestDistance(const Vector2D &otherPoint) const -> double
{
    return closestDistanceLocal(transform.toLocal(otherPoint));
}

auto Surface2::closestIntersection(const Ray2D &ray) const -> SurfaceRayIntersection2
{
    auto result = closestIntersectionLocal(transform.toLocal(ray));
    result.point = transform.toWorld(result.point);
    result.normal = transform.toWorldDirection(result.normal);
    result.normal *= (isNormalFlipped) ? -1.0 : 1.0;
    return result;
}

auto Surface2::closestNormal(const Vector2D &otherPoint) const -> Vector2D
{
    auto result = transform.toWorldDirection(closestNormalLocal(transform.toLocal(otherPoint)));
    result *= (isNormalFlipped) ? -1.0 : 1.0;
    return result;
}

void Surface2::updateQueryEngine()
{
    // Do nothing
}

auto Surface2::isBounded() const -> bool { return true; }

auto Surface2::isValidGeometry() const -> bool { return true; }

auto Surface2::isInside(const Vector2D &otherPoint) const -> bool
{
    return isNormalFlipped == !isInsideLocal(transform.toLocal(otherPoint));
}

auto Surface2::intersectsLocal(const Ray2D &rayLocal) const -> bool
{
    auto result = closestIntersectionLocal(rayLocal);
    return result.isIntersecting;
}

auto Surface2::closestDistanceLocal(const Vector2D &otherPointLocal) const -> double
{
    return otherPointLocal.distanceTo(closestPointLocal(otherPointLocal));
}

auto Surface2::isInsideLocal(const Vector2D &otherPointLocal) const -> bool
{
    Vector2D cpLocal = closestPointLocal(otherPointLocal);
    Vector2D normalLocal = closestNormalLocal(otherPointLocal);
    return (otherPointLocal - cpLocal).dot(normalLocal) < 0.0;
}
