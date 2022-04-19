#ifndef HINAPE_INDEXED_TRIANGLE_MESH_H
#define HINAPE_INDEXED_TRIANGLE_MESH_H

#include "../../Common/common.h"

namespace HinaPE::HinaGeom
{
    class IndexedTriangleMesh
    {
    protected:
        explicit IndexedTriangleMesh() = default;
    public:
        virtual ~IndexedTriangleMesh() = default;
    };
}

#endif //HINAPE_INDEXED_TRIANGLE_MESH_H
