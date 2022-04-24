#ifndef HINAPE_VERTEX_INFO_H
#define HINAPE_VERTEX_INFO_H

#include "../../Common/common.h"
#include "../../Common/validationable.h"

namespace HinaPE::HinaGeom
{
    class VertexInfo : public Validationable
    {
    public:
        virtual ~VertexInfo() = default;
        void build();
    protected:
        explicit VertexInfo(Vertex3Matrix vertices);
    protected:
        Vertex3Matrix V_;
        NormalMatrix N_;
        std::vector<std::vector<unsigned int>> VE_; // Vertex-edges - size = [#vertices x ?], row = [edge1_index, edge2_index, ... ...]
        std::vector<std::vector<unsigned int>> VF_; // Vertex-faces - size = [#faces x ?], row = [face1_index, face2_index, ... ...]
        bool is_closed_; // is a closed_mesh?

    private:
        void _validation_check() final;
    };
}

#endif //HINAPE_VERTEX_INFO_H
