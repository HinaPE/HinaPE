#ifndef HINAPE_PBD_SOLVER_H
#define HINAPE_PBD_SOLVER_H

#include "../rigidbody_solver.h"

namespace HinaPE::RigidBodyEngine
{
class PBDSolver : public RigidBodySolver
{

};
using PBDSolverPtr = std::shared_ptr<PBDSolver>;
}

#endif //HINAPE_PBD_SOLVER_H
