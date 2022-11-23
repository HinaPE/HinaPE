#include "point_generator3.h"

#include "pch.h"

namespace HinaPE::FluidEngine
{

PointGenerator3::PointGenerator3()
= default;

PointGenerator3::~PointGenerator3()
= default;

void PointGenerator3::generate(const BoundingBox3D &boundingBox, double spacing, Array1<Vector3D> *points) const
{
    forEachPoint(boundingBox, spacing, [&points](const Vector3D &point)
    {
        points->append(point);
        return true;
    });
}

}  // namespace HinaPE::FluidEngine
