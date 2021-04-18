#ifndef ASSIGNMENT_FRAMEWORK_COLLISION_H
#define ASSIGNMENT_FRAMEWORK_COLLISION_H

#include "PhysicalObjects.h"

enum COLLISION_AXIS {
    NO_COLLIDED,
    AXIS_X,
    AXIS_Y,
    AXIS_Z
};

class Collision {
public:

    static void naive_aabb_collison2D(const std::unique_ptr<BasicPhysicalObjects> &obj,
                                      const std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs,
                                      float delta_x, float delta_y, COLLISION_AXIS &axis, float &optimal_f);

    static void naive_aabb_collison3D(const std::unique_ptr<BasicPhysicalObjects> &obj,
                                      const std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs,
                                      float delta_x, float delta_y, float delta_z, COLLISION_AXIS &axis, float &optimal_f);
};

#endif //ASSIGNMENT_FRAMEWORK_COLLISION_H
