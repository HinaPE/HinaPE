#include "point_generator2.h"

#include "pch.h"

namespace HinaPE::Fluid
{

PointGenerator2::PointGenerator2()
= default;

PointGenerator2::~PointGenerator2()
= default;

void PointGenerator2::generate(const BoundingBox2D &boundingBox, double spacing, Array1<Vector2D> *points) const
{
    forEachPoint(boundingBox, spacing, [&points](const Vector2D &point)
    {
        points->append(point);
        return true;
    });
}

}  // namespace HinaPE::Fluid
