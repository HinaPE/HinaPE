#ifndef ASSIGNMENT_FRAMEWORK_COLLISION_H
#define ASSIGNMENT_FRAMEWORK_COLLISION_H

#include "PhysicalObjects.h"

class Collision {
public:

    static void naive_aabb_collison(std::unique_ptr<BasicPhysicalObjects> &obj,
                                    std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs);
};

#endif //ASSIGNMENT_FRAMEWORK_COLLISION_H
