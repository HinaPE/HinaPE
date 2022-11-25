#ifndef HINAPE_PBD_SOLVER_H
#define HINAPE_PBD_SOLVER_H

#include "../rigid_solver.h"

namespace HinaPE::Rigid
{
class PBDSolver : public RigidSolver
{

};
using PBDSolverPtr = std::shared_ptr<PBDSolver>;
}

#endif //HINAPE_PBD_SOLVER_H
