#ifndef HINAPE_CLOTH_GEOMETRY_DATA_H
#define HINAPE_CLOTH_GEOMETRY_DATA_H

#include "array/array1.h"
#include "math/vector3.h"

#include <memory>
#include <vector>

namespace HinaPE::ClothEngine
{
class ClothGeometryData
{
private:
    std::vector<Vector3D> _vertices;
    std::vector<Vector3D> _normals;
    std::vector<Vector3D> _colors;
    std::vector<Vector3D> _uvs;
    std::vector<Vector3D> _tangents;
    std::vector<Vector3D> _bi_tangents;
    std::vector<unsigned int> _indices;
};
using ClothGeometryDataPtr = std::shared_ptr<ClothGeometryData>;
}

#endif //HINAPE_CLOTH_GEOMETRY_DATA_H
