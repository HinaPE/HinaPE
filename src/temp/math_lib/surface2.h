// Copyright (c) 2018 Doyub Kim
//
// I am making my contributions/submissions to this project solely in my
// personal capacity and am not conveying any rights to any intellectual
// property of any third parties.

#ifndef INCLUDE_JET_SURFACE2_H_
#define INCLUDE_JET_SURFACE2_H_

#include "bounding_box2.h"
#include "constants.h"
#include "ray2.h"
#include "transform2.h"
#include <memory>

namespace jet
{

//! Struct that represents ray-surface intersection point.
struct SurfaceRayIntersection2
{
    bool isIntersecting = false;
    double distance = kMaxD;
    Vector2D point;
    Vector2D normal;
};

//! Abstract base class for 2-D surface.
class Surface2
{
public:
    //! Local-to-world transform.
    Transform2 transform;

    //! Flips normal.
    bool isNormalFlipped = false;

    //! Constructs a surface with normal direction.
    explicit Surface2(Transform2 transform = Transform2(), bool isNormalFlipped = false);

    //! Copy constructor.
    Surface2(const Surface2 &other);

    //! Default destructor.
    virtual ~Surface2();

    //! Returns the closest point from the given point \p otherPoint to the
    //! surface.
    auto closestPoint(const Vector2D &otherPoint) const -> Vector2D;

    //! Returns the bounding box of this surface object.
    auto boundingBox() const -> BoundingBox2D;

    //! Returns true if the given \p ray intersects with this surface object.
    auto intersects(const Ray2D &ray) const -> bool;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface.
    auto closestDistance(const Vector2D &otherPoint) const -> double;

    //! Returns the closest intersection point for given \p ray.
    auto closestIntersection(const Ray2D &ray) const -> SurfaceRayIntersection2;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint.
    auto closestNormal(const Vector2D &otherPoint) const -> Vector2D;

    //! Updates internal spatial query engine.
    virtual void updateQueryEngine();

    //! Returns true if bounding box can be defined.
    virtual auto isBounded() const -> bool;

    //! Returns true if the surface is a valid geometry.
    virtual auto isValidGeometry() const -> bool;

    //! Returns true if \p otherPoint is inside the volume defined by the
    //! surface.
    auto isInside(const Vector2D &otherPoint) const -> bool;

protected:
    //! Returns the closest point from the given point \p otherPoint to the
    //! surface in local frame.
    virtual auto closestPointLocal(const Vector2D &otherPoint) const -> Vector2D = 0;

    //! Returns the bounding box of this surface object in local frame.
    virtual auto boundingBoxLocal() const -> BoundingBox2D = 0;

    //! Returns the closest intersection point for given \p ray in local frame.
    virtual auto closestIntersectionLocal(const Ray2D &ray) const -> SurfaceRayIntersection2 = 0;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint in local frame.
    virtual auto closestNormalLocal(const Vector2D &otherPoint) const -> Vector2D = 0;

    //! Returns true if the given \p ray intersects with this surface object
    //! in local frame.
    virtual auto intersectsLocal(const Ray2D &ray) const -> bool;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface in local frame.
    virtual auto closestDistanceLocal(const Vector2D &otherPoint) const -> double;

    //! Returns true if \p otherPoint is inside by given \p depth the volume
    //! defined by the surface in local frame.
    virtual auto isInsideLocal(const Vector2D &otherPoint) const -> bool;
};

//! Shared pointer for the Surface2 type.
using Surface2Ptr = std::shared_ptr<Surface2>;

//!
//! \brief Base class for 2-D surface builder.
//!
template<typename DerivedBuilder>
class SurfaceBuilderBase2
{
public:
    //! Returns builder with flipped normal flag.
    auto withIsNormalFlipped(bool isNormalFlipped) -> DerivedBuilder &;

    //! Returns builder with translation.
    auto withTranslation(const Vector2D &translation) -> DerivedBuilder &;

    //! Returns builder with orientation.
    auto withOrientation(double orientation) -> DerivedBuilder &;

    //! Returns builder with transform.
    auto withTransform(const Transform2 &transform) -> DerivedBuilder &;

protected:
    bool _isNormalFlipped = false;
    Transform2 _transform;
};

template<typename T>
auto SurfaceBuilderBase2<T>::withIsNormalFlipped(bool isNormalFlipped) -> T &
{
    _isNormalFlipped = isNormalFlipped;
    return static_cast<T &>(*this);
}

template<typename T>
auto SurfaceBuilderBase2<T>::withTranslation(const Vector2D &translation) -> T &
{
    _transform.setTranslation(translation);
    return static_cast<T &>(*this);
}

template<typename T>
auto SurfaceBuilderBase2<T>::withOrientation(double orientation) -> T &
{
    _transform.setOrientation(orientation);
    return static_cast<T &>(*this);
}

template<typename T>
auto SurfaceBuilderBase2<T>::withTransform(const Transform2 &transform) -> T &
{
    _transform = transform;
    return static_cast<T &>(*this);
}

}  // namespace jet

#endif  // INCLUDE_JET_SURFACE2_H_
