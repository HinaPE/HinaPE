#include "cloth_geometry_data.h"

HinaPE::ClothEngine::ClothGeometryData::ClothGeometryData(HinaPE::ClothEngine::real width, HinaPE::ClothEngine::real height, size_t num_x, size_t num_y)
{
    _vertices.resize(num_x * num_y);
    _indices.resize((num_x - 1) * (num_y - 1) * 6);
    for (size_t j = 0; j < num_y; ++j)
        for (size_t i = 0; i < num_x; ++i)
            _vertices[j * num_x + i] = mVector3(width * (i / (num_x - 1.0)), height * (j / (num_y - 1.0)), 0.0);
    for (size_t j = 0; j < num_y - 1; ++j)
    {
        for (size_t i = 0; i < num_x - 1; ++i)
        {
            _indices[(j * (num_x - 1) + i) * 6 + 0] = j * num_x + i;
            _indices[(j * (num_x - 1) + i) * 6 + 1] = j * num_x + i + 1;
            _indices[(j * (num_x - 1) + i) * 6 + 2] = (j + 1) * num_x + i;
            _indices[(j * (num_x - 1) + i) * 6 + 3] = (j + 1) * num_x + i;
            _indices[(j * (num_x - 1) + i) * 6 + 4] = j * num_x + i + 1;
            _indices[(j * (num_x - 1) + i) * 6 + 5] = (j + 1) * num_x + i + 1;
        }
    }
}
auto HinaPE::ClothEngine::ClothGeometryData::get_vertices() const -> const std::vector<HinaPE::ClothEngine::mVector3> & { return _vertices; }
auto HinaPE::ClothEngine::ClothGeometryData::get_indices() const -> const std::vector<unsigned int> & { return _indices; }
