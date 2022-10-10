// Copyright (c) 2018 Doyub Kim
//
// I am making my contributions/submissions to this project solely in my
// personal capacity and am not conveying any rights to any intellectual
// property of any third parties.

#ifndef INCLUDE_JET_SURFACE3_H_
#define INCLUDE_JET_SURFACE3_H_

#include "bounding_box3.h"
#include "constants.h"
#include "ray3.h"
#include "transform3.h"
#include <memory>

namespace jet
{

//! Struct that represents ray-surface intersection point.
struct SurfaceRayIntersection3
{
    bool isIntersecting = false;
    double distance = kMaxD;
    Vector3D point;
    Vector3D normal;
};

//! Abstract base class for 3-D surface.
class Surface3
{
public:
    //! Local-to-world transform.
    Transform3 transform;

    //! Flips normal when calling Surface3::closestNormal(...).
    bool isNormalFlipped = false;

    //! Constructs a surface with normal direction.
    explicit Surface3(Transform3 transform = Transform3(), bool isNormalFlipped = false);

    //! Copy constructor.
    Surface3(const Surface3 &other);

    //! Default destructor.
    virtual ~Surface3();

    //! Returns the closest point from the given point \p otherPoint to the
    //! surface.
    auto closestPoint(const Vector3D &otherPoint) const -> Vector3D;

    //! Returns the bounding box of this surface object.
    auto boundingBox() const -> BoundingBox3D;

    //! Returns true if the given \p ray intersects with this surface object.
    auto intersects(const Ray3D &ray) const -> bool;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface.
    auto closestDistance(const Vector3D &otherPoint) const -> double;

    //! Returns the closest intersection point for given \p ray.
    auto closestIntersection(const Ray3D &ray) const -> SurfaceRayIntersection3;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint.
    auto closestNormal(const Vector3D &otherPoint) const -> Vector3D;

    //! Updates internal spatial query engine.
    virtual void updateQueryEngine();

    //! Returns true if bounding box can be defined.
    virtual auto isBounded() const -> bool;

    //! Returns true if the surface is a valid geometry.
    virtual auto isValidGeometry() const -> bool;

    //! Returns true if \p otherPoint is inside the volume defined by the
    //! surface.
    auto isInside(const Vector3D &otherPoint) const -> bool;

protected:
    //! Returns the closest point from the given point \p otherPoint to the
    //! surface in local frame.
    virtual auto closestPointLocal(const Vector3D &otherPoint) const -> Vector3D = 0;

    //! Returns the bounding box of this surface object in local frame.
    virtual auto boundingBoxLocal() const -> BoundingBox3D = 0;

    //! Returns the closest intersection point for given \p ray in local frame.
    virtual auto closestIntersectionLocal(const Ray3D &ray) const -> SurfaceRayIntersection3 = 0;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint in local frame.
    virtual auto closestNormalLocal(const Vector3D &otherPoint) const -> Vector3D = 0;

    //! Returns true if the given \p ray intersects with this surface object
    //! in local frame.
    virtual auto intersectsLocal(const Ray3D &ray) const -> bool;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface in local frame.
    virtual auto closestDistanceLocal(const Vector3D &otherPoint) const -> double;

    //! Returns true if \p otherPoint is inside by given \p depth the volume
    //! defined by the surface in local frame.
    virtual auto isInsideLocal(const Vector3D &otherPoint) const -> bool;
};

//! Shared pointer for the Surface3 type.
using Surface3Ptr = std::shared_ptr<Surface3>;

//!
//! \brief Base class for 3-D surface builder.
//!
template<typename DerivedBuilder>
class SurfaceBuilderBase3
{
public:
    //! Returns builder with flipped normal flag.
    auto withIsNormalFlipped(bool isNormalFlipped) -> DerivedBuilder &;

    //! Returns builder with translation.
    auto withTranslation(const Vector3D &translation) -> DerivedBuilder &;

    //! Returns builder with orientation.
    auto withOrientation(const QuaternionD &orientation) -> DerivedBuilder &;

    //! Returns builder with transform.
    auto withTransform(const Transform3 &transform) -> DerivedBuilder &;

protected:
    bool _isNormalFlipped = false;
    Transform3 _transform;
};

template<typename T>
auto SurfaceBuilderBase3<T>::withIsNormalFlipped(bool isNormalFlipped) -> T &
{
    _isNormalFlipped = isNormalFlipped;
    return static_cast<T &>(*this);
}

template<typename T>
auto SurfaceBuilderBase3<T>::withTranslation(const Vector3D &translation) -> T &
{
    _transform.setTranslation(translation);
    return static_cast<T &>(*this);
}

template<typename T>
auto SurfaceBuilderBase3<T>::withOrientation(const QuaternionD &orientation) -> T &
{
    _transform.setOrientation(orientation);
    return static_cast<T &>(*this);
}

template<typename T>
auto SurfaceBuilderBase3<T>::withTransform(const Transform3 &transform) -> T &
{
    _transform = transform;
    return static_cast<T &>(*this);
}

}  // namespace jet

#endif  // INCLUDE_JET_SURFACE3_H_
