#ifndef HINAPE_SPHERE_H
#define HINAPE_SPHERE_H

#include "triangle_meshes/vertex_info.h"
#include "triangle_meshes/indexed_triangle_mesh.h"

namespace HinaPE::HinaGeom
{
    class Sphere : protected VertexInfo, protected IndexedTriangleMesh
    {
    public:
        static Sphere *build_Sphere(Real radius);
    protected:
        Sphere(Vertex3Matrix V, TriFaceMatrix F);
    };
}


#endif //HINAPE_SPHERE_H
