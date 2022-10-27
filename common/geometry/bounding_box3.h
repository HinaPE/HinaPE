#ifndef HINAPE_FLUID_ENGINE_BOUNDING_BOX3_H_
#define HINAPE_FLUID_ENGINE_BOUNDING_BOX3_H_

#include "bounding_box.h"
#include "ray3.h"

#include "math/vector3.h"

#include <limits>

namespace HinaPE::FluidEngine
{

//!
//! \brief      3-D box-ray intersection result.
//!
//! \tparam     T     The value type.
//!
template<typename T>
struct BoundingBoxRayIntersection3
{
    //! True if the box and ray intersects.
    bool isIntersecting = false;

    //! Distance to the first intersection point.
    T tNear = std::numeric_limits<T>::max();

    //! Distance to the second (and the last) intersection point.
    T tFar = std::numeric_limits<T>::max();
};

//!
//! \brief 3-D axis-aligned bounding box class.
//!
//! \tparam T - Real number type.
//! \tparam N - Dimension.
//!
template<typename T>
class BoundingBox<T, 3>
{
public:
    //! Lower corner of the bounding box.
    Vector3<T> lowerCorner;

    //! Upper corner of the bounding box.
    Vector3<T> upperCorner;

    //! Default constructor.
    BoundingBox();

    //! Constructs a box that tightly covers two points.
    BoundingBox(const Vector3<T> &point1, const Vector3<T> &point2);

    //! Constructs a box with other box instance.
    BoundingBox(const BoundingBox &other);

    //! Returns width of the box.
    auto width() const -> T;

    //! Returns height of the box.
    auto height() const -> T;

    //! Returns depth of the box.
    auto depth() const -> T;

    //! Returns length of the box in given axis.
    auto length(size_t axis) -> T;

    //! Returns true of this box and other box overlaps.
    auto overlaps(const BoundingBox &other) const -> bool;

    //! Returns true if the input vector is inside of this box.
    auto contains(const Vector3<T> &point) const -> bool;

    //! Returns true if the input ray is intersecting with this box.
    auto intersects(const Ray3<T> &ray) const -> bool;

    //! Returns intersection.isIntersecting = true if the input ray is
    //! intersecting with this box. If interesects, intersection.tNear is
    //! assigned with distant to the closest intersecting point, and
    //! intersection.tFar with furthest.
    auto closestIntersection(const Ray3<T> &ray) const -> BoundingBoxRayIntersection3<T>;

    //! Returns the mid-point of this box.
    auto midPoint() const -> Vector3<T>;

    //! Returns diagonal length of this box.
    auto diagonalLength() const -> T;

    //! Returns squared diagonal length of this box.
    auto diagonalLengthSquared() const -> T;

    //! Resets this box to initial state (min=infinite, max=-infinite).
    void reset();

    //! Merges this and other point.
    void merge(const Vector3<T> &point);

    //! Merges this and other box.
    void merge(const BoundingBox &other);

    //! Expands this box by given delta to all direction.
    //! If the width of the box was x, expand(y) will result a box with
    //! x+y+y width.
    void expand(T delta);

    //! Returns corner position. Index starts from x-first order.
    auto corner(size_t idx) const -> Vector3<T>;

    //! Returns the clamped point.
    auto clamp(const Vector3<T> &point) const -> Vector3<T>;

    //! Returns true if the box is empty.
    auto isEmpty() const -> bool;
};

//! Type alias for 3-D BoundingBox.
template<typename T> using BoundingBox3 = BoundingBox<T, 3>;

//! Float-type 3-D BoundingBox.
using BoundingBox3F = BoundingBox3<float>;

//! Double-type 3-D BoundingBox.
using BoundingBox3D = BoundingBox3<double>;

//! Float-type 3-D box-ray intersection result.
using BoundingBoxRayIntersection3F = BoundingBoxRayIntersection3<float>;

//! Double-type 3-D box-ray intersection result.
using BoundingBoxRayIntersection3D = BoundingBoxRayIntersection3<double>;

}  // namespace HinaPE::FluidEngine

#include "bounding_box3-inl.h"

#endif  // HINAPE_FLUID_ENGINE_BOUNDING_BOX3_H_
