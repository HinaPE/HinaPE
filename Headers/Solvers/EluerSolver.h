#ifndef ASSIGNMENTFRAMEWORK_ELUERSOLVER_H
#define ASSIGNMENTFRAMEWORK_ELUERSOLVER_H

#include "Eigen/Eigen"

#include "Objects2D.h"

class EluerSolver {
public:
    static void ForwardEuler(std::unique_ptr<BasicPhysicalObjects> &obj, std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs, float delta_time);

    static void SemiBackwardEuler(std::unique_ptr<BasicPhysicalObjects> &obj, std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs, float delta_time);

    static void BackwardEuler(std::unique_ptr<BasicPhysicalObjects> &obj, std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs, float delta_time);

};

#endif //ASSIGNMENTFRAMEWORK_ELUERSOLVER_H
