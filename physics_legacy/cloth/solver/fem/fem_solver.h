#ifndef HINAPE_FEM_SOLVER_H
#define HINAPE_FEM_SOLVER_H

#include "../cloth_solver.h"
#include <memory>

namespace HinaPE::Cloth
{
class FEMSolver : public ClothSolver
{
public:
    void step(real dt) final;
};
using FEMSolverPtr = std::shared_ptr<FEMSolver>;
}

#endif //HINAPE_FEM_SOLVER_H
