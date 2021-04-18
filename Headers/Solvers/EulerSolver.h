#ifndef ASSIGNMENTFRAMEWORK_EULERSOLVER_H
#define ASSIGNMENTFRAMEWORK_EULERSOLVER_H

#include "Objects2D.h"
#include "Objects3D.h"

#include "Collision.h"


#include "MassSpringTEMPLATE.h"

class EulerSolver {
public:
    static void ForwardEuler2D(std::unique_ptr<BasicPhysicalObjects> &obj,
                               std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs, float delta_time);

    static void ForwardEuler3D(std::unique_ptr<BasicPhysicalObjects> &obj,
                               std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs, float delta_time);

    static void ImplicitEuler2D(std::unique_ptr<BasicPhysicalObjects> &obj,
                                std::vector<std::unique_ptr<BasicPhysicalObjects>> &world_objs, float delta_time);

    static void ForwardEulerMassSpring(std::unique_ptr<MassSpringTEMPLATE> &mass_spring_system, float delta_time);
};

#endif //ASSIGNMENTFRAMEWORK_EULERSOLVER_H
