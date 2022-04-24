#ifndef HINAPE_INDEXED_TRIANGLE_MESH_H
#define HINAPE_INDEXED_TRIANGLE_MESH_H

#include "../../Common/common.h"
#include "../../Common/validationable.h"

namespace HinaPE::HinaGeom
{
    class IndexedTriangleMesh : public Validationable
    {
    public:
        virtual ~IndexedTriangleMesh() = default;
    protected:
        explicit IndexedTriangleMesh(TriFaceMatrix faces);
        void build();
    private:
        void _validation_check() override;

    protected:
        TriFaceMatrix F_;
        EdgeMatrix E_;
        std::vector<std::vector<unsigned int>> FE_; // Face-edges - size = [#faces x 3], row = [edge1_index, edge2_index, edge3_index]
        std::vector<std::vector<unsigned int>> EVF_; // Edge-vertices-faces - size = [#edges x 4], row = [v1_index, v2_index, face1_index, face2_index]
    };
}

#endif //HINAPE_INDEXED_TRIANGLE_MESH_H
