#ifndef ASSIGNMENT_FRAMEWORK_COLLISION_H
#define ASSIGNMENT_FRAMEWORK_COLLISION_H

#include "PhysicalObjects.h"

enum DIRECT_POINT {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

enum COLLISION_AXIS {
    NO_COLLIDED,
    AXIS_X,
    AXIS_Y
};

class Collision {
public:

    static void naive_aabb_collison(const std::unique_ptr<BasicPhysicalObjects> &obj,
                                    const std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs,
                                    float delta_x, float delta_y, COLLISION_AXIS &axis, float &optimal_f);
};

#endif //ASSIGNMENT_FRAMEWORK_COLLISION_H
