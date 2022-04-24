#include "indexed_triangle_mesh.h"

#include <utility>

using namespace HinaPE::HinaGeom;

IndexedTriangleMesh::IndexedTriangleMesh(TriFaceMatrix faces) : F_(std::move(faces))
{
    auto faces_num = F_.size();
    FE_.resize(faces_num);
}

void IndexedTriangleMesh::build()
{
    // TODO:

    validation_check();
}

void IndexedTriangleMesh::_validation_check()
{
    assert(FE_.size() == F_.size());
    assert(EVF_.size() == E_.size());
}
