#ifndef HINAPE_CLOTH_ENGINE_CLOTH_RENDER_DATA_H
#define HINAPE_CLOTH_ENGINE_CLOTH_RENDER_DATA_H

#include "array/array1.h"
#include "math/vector3.h"

#include <memory>
#include <vector>

namespace HinaPE::ClothEngine
{
class ClothRenderData
{
public:

private:
    std::vector<Vector3D> _vertices;
    std::vector<Vector3D> _normals;
    std::vector<Vector3D> _colors;
    std::vector<Vector3D> _uvs;
    std::vector<Vector3D> _tangents;
    std::vector<Vector3D> _bi_tangents;
    std::vector<unsigned int> _indices;
};
using ClothRenderDataPtr = std::shared_ptr<ClothRenderData>;
}

#endif //HINAPE_CLOTH_ENGINE_CLOTH_RENDER_DATA_H
