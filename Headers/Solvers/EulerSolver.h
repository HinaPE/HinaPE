#ifndef ASSIGNMENTFRAMEWORK_EULERSOLVER_H
#define ASSIGNMENTFRAMEWORK_EULERSOLVER_H

#include "Eigen/Eigen"

#include "Objects2D.h"

#include "Collision.h"

class EulerSolver {
public:
    static void ForwardEuler(std::unique_ptr<BasicPhysicalObjects> &obj,
                             std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs, float delta_time);
};

#endif //ASSIGNMENTFRAMEWORK_EULERSOLVER_H
