#ifndef HINAPE_FLUID_ENGINE_BOUNDING_BOX_H_
#define HINAPE_FLUID_ENGINE_BOUNDING_BOX_H_

#include "math/vector.h"

namespace HinaPE::FluidEngine
{

//!
//! \brief Generic N-D axis-aligned bounding box class.
//!
//! \tparam T - Real number type.
//! \tparam N - Dimension.
//!
template<typename T, size_t N>
class BoundingBox
{
public:
    static_assert(N > 0, "Size of static-sized box should be greater than zero.");

    using VectorType = Vector<T, N>;

    //! Lower corner of the bounding box.
    VectorType lowerCorner;

    //! Upper corner of the bounding box.
    VectorType upperCorner;

    //! Default constructor.
    BoundingBox();

    //! Constructs a box that tightly covers two points.
    BoundingBox(const VectorType &point1, const VectorType &point2);

    //! Constructs a box with other box instance.
    BoundingBox(const BoundingBox &other);

    //! Returns true of this box and other box overlaps.
    auto overlaps(const BoundingBox &other) const -> bool;

    //! Returns true if the input point is inside of this box.
    auto contains(const VectorType &point) const -> bool;

    //! Returns the mid-point of this box.
    auto midPoint() const -> VectorType;

    //! Returns diagonal length of this box.
    auto diagonalLength() const -> T;

    //! Returns squared diagonal length of this box.
    auto diagonalLengthSquared() const -> T;

    //! Resets this box to initial state (min=infinite, max=-infinite).
    void reset();

    //! Merges this and other point.
    void merge(const VectorType &point);

    //! Merges this and other boxes.
    void merge(const BoundingBox &other);

    //! Expands this box by given delta to all direction.
    //! If the width of the box was x, expand(y) will result a box with
    //! x+y+y width.
    void expand(T delta);
};

}  // namespace HinaPE::FluidEngine

#include "bounding_box-inl.h"

#endif  // HINAPE_FLUID_ENGINE_BOUNDING_BOX_H_
