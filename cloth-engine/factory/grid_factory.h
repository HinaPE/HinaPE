#ifndef HINAPE_GRID_FACTORY_H
#define HINAPE_GRID_FACTORY_H

#include "util/math_api.h"

namespace HinaPE::ClothEngine
{
class GridFactory
{
public:
    static void create_grid_pos_x_oriented(real width, real height, int rows, int cols, const Vector3 &center_point = Vector3(0, 0, 0));
    static void create_grid_pos_y_oriented(real width, real height, int rows, int cols, const Vector3 &center_point = Vector3(0, 0, 0));
    static void create_grid_pos_z_oriented(real width, real height, int rows, int cols, const Vector3 &center_point = Vector3(0, 0, 0));
    static void create_grid_neg_x_oriented(real width, real height, int rows, int cols, const Vector3 &center_point = Vector3(0, 0, 0));
    static void create_grid_neg_y_oriented(real width, real height, int rows, int cols, const Vector3 &center_point = Vector3(0, 0, 0));
    static void create_grid_neg_z_oriented(real width, real height, int rows, int cols, const Vector3 &center_point = Vector3(0, 0, 0));
    static void create_grid(const Vector3 &center_point, const Vector3 &orientation, real width, real height, int rows, int cols);
};
}

#endif //HINAPE_GRID_FACTORY_H
