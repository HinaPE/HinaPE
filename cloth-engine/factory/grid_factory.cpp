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
        auto index = i * cols + j;
        real elem1 = as_real(j) / (as_real(cols - 1)) * width - width / 2.0f;
        real elem2 = as_real(i) / (as_real(rows - 1)) * height - height / 2.0f;
        Vector3 res;

        _nodes[index] = res;
    });
}
