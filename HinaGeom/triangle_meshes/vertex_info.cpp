#include "vertex_info.h"

#include <utility>

using namespace HinaPE::HinaGeom;

VertexInfo::VertexInfo(Vertex3Matrix vertices) : V_(std::move(vertices)), is_closed_(false)
{
    auto vertices_num = V_.rows();
    N_.resize(vertices_num, N_.cols());
    VE_.resize(vertices_num);
    VF_.resize(vertices_num);
}

void VertexInfo::build()
{
    // TODO:

    validation_check();
}

void VertexInfo::_validation_check()
{
    assert(V_.rows() > 0);
    assert(V_.cols() == 3);
    assert(N_.rows() == V_.rows());
    assert(N_.cols() == 3);
    assert(VE_.size() == V_.rows());
    assert(VF_.size() == V_.rows());
}
