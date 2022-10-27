#include "triangle_mesh_to_sdf.h"

#include "array/array3.h"
#include "array/array_utils.h"

#include "pch.h"

#include <algorithm>
#include <vector>

using namespace HinaPE::FluidEngine;

namespace HinaPE::FluidEngine
{

void triangleMeshToSdf(const TriangleMesh3 &mesh, ScalarGrid3 *sdf, const unsigned int)
{
    Size3 size = sdf->dataSize();
    if (size.x * size.y * size.z == 0)
    {
        return;
    }

    const auto pos = sdf->dataPosition();
    mesh.updateQueryEngine();
    sdf->parallelForEachDataPointIndex([&](size_t i, size_t j, size_t k)
                                       {
                                           const Vector3D p = pos(i, j, k);
                                           const double d = mesh.closestDistance(p);
                                           const double sd = mesh.isInside(p) ? -d : d;
                                           (*sdf)(i, j, k) = sd;
                                       });
}

}  // namespace HinaPE::FluidEngine
