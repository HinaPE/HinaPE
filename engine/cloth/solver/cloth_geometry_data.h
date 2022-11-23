#ifndef HINAPE_CLOTH_GEOMETRY_DATA_H
#define HINAPE_CLOTH_GEOMETRY_DATA_H

#include "../math_api.h"

#include <memory>
#include <vector>

namespace HinaPE::ClothEngine
{
class ClothGeometryData
{
public:
    auto get_vertices() const -> const std::vector<mVector3> &;
    auto get_indices() const -> const std::vector<unsigned int> &;

private:
    std::vector<mVector3> _vertices;
    std::vector<unsigned int> _indices;

public:
    ClothGeometryData() = default;
    ClothGeometryData(real width, real height, size_t num_x, size_t num_y);
    ClothGeometryData(const ClothGeometryData &) = delete;
    ClothGeometryData(ClothGeometryData &&) = default;
    ~ClothGeometryData() = default;
    auto operator=(const ClothGeometryData &) -> ClothGeometryData & = delete;
    auto operator=(ClothGeometryData &&) -> ClothGeometryData & = default;
};
using ClothGeometryDataPtr = std::shared_ptr<ClothGeometryData>;
}

#endif //HINAPE_CLOTH_GEOMETRY_DATA_H
