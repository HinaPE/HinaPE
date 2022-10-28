#ifndef HINAPE_FLUID_ENGINE_TRIANGLE_POINT_GENERATOR_H_
#define HINAPE_FLUID_ENGINE_TRIANGLE_POINT_GENERATOR_H_

#include "point_generator2.h"

namespace HinaPE::FluidEngine
{

//!
//! \brief Right triangle point generator.
//!
class TrianglePointGenerator final : public PointGenerator2
{
public:
    //!
    //! \brief Invokes \p callback function for each right triangle points
    //! inside \p boundingBox.
    //!
    //! This function iterates every right triangle points inside \p boundingBox
    //! where \p spacing is the size of the right triangle structure.
    //!
    void forEachPoint(const BoundingBox2D &boundingBox, double spacing, const std::function<bool(const Vector2D &)> &callback) const override;
};

using TrianglePointGeneratorPtr = std::shared_ptr<TrianglePointGenerator>;

}  // namespace HinaPE::FluidEngine

#endif  // HINAPE_FLUID_ENGINE_TRIANGLE_POINT_GENERATOR_H_
