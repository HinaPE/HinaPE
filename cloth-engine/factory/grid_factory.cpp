#include "grid_factory.h"
#include <vector>

void HinaPE::ClothEngine::GridFactory::create_grid_pos_x_oriented(real width, real height, int rows, int cols, const HinaPE::ClothEngine::Vector3 &center_point) { create_grid(center_point, as_real_vector3(1, 0, 0), width, height, rows, cols); }
void HinaPE::ClothEngine::GridFactory::create_grid_pos_y_oriented(real width, real height, int rows, int cols, const HinaPE::ClothEngine::Vector3 &center_point) { create_grid(center_point, as_real_vector3(0, 1, 0), width, height, rows, cols); }
void HinaPE::ClothEngine::GridFactory::create_grid_pos_z_oriented(real width, real height, int rows, int cols, const HinaPE::ClothEngine::Vector3 &center_point) { create_grid(center_point, as_real_vector3(0, 0, 1), width, height, rows, cols); }
void HinaPE::ClothEngine::GridFactory::create_grid_neg_x_oriented(real width, real height, int rows, int cols, const HinaPE::ClothEngine::Vector3 &center_point) { create_grid(center_point, as_real_vector3(-1, 0, 0), width, height, rows, cols); }
void HinaPE::ClothEngine::GridFactory::create_grid_neg_y_oriented(real width, real height, int rows, int cols, const HinaPE::ClothEngine::Vector3 &center_point) { create_grid(center_point, as_real_vector3(0, -1, 0), width, height, rows, cols); }
void HinaPE::ClothEngine::GridFactory::create_grid_neg_z_oriented(real width, real height, int rows, int cols, const HinaPE::ClothEngine::Vector3 &center_point) { create_grid(center_point, as_real_vector3(0, 0, -1), width, height, rows, cols); }
void HinaPE::ClothEngine::GridFactory::create_grid(const HinaPE::ClothEngine::Vector3 &center_point, const HinaPE::ClothEngine::Vector3 &orientation, real width, real height, int rows, int cols)
{
    std::vector<Vector3> _nodes;
    std::vector<int> _indices;

    _nodes.resize(rows * cols);
    HinaPE::FluidEngine::parallelFor(0, rows, 0, cols, [&](size_t i, size_t j)
    {
        _nodes[i * cols + j] = as_real_vector3(j, i, 0);

    });
}
