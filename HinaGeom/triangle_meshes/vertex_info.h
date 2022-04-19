#ifndef HINAPE_VERTEX_INFO_H
#define HINAPE_VERTEX_INFO_H

#include "../../Common/common.h"

namespace HinaPE::HinaGeom
{
    class VertexInfo
    {


    public:
        virtual ~VertexInfo() = default;
    protected:
        explicit VertexInfo() = default;

    protected:
        Vertex3Matrix vertices_;
    };
}

#endif //HINAPE_VERTEX_INFO_H
