#ifndef HINAPE_SPHERE_H
#define HINAPE_SPHERE_H

#include "triangle_meshes/vertex_info.h"
#include "triangle_meshes/indexed_triangle_mesh.h"

#include <tuple>

namespace HinaPE::HinaGeom
{
    class Sphere
    {
        static std::tuple<VertexInfo *, IndexedTriangleMesh *> build_Sphere(Real radius);
    };
}


#endif //HINAPE_SPHERE_H
