#ifndef HINAPE_PD_SOLVER_H
#define HINAPE_PD_SOLVER_H

#include "../cloth_solver.h"
#include <memory>

namespace HinaPE::Cloth
{
class PDSolver : public ClothSolver
{
public:
    void step(real dt) final;
};
using PDSolverPtr = std::shared_ptr<PDSolver>;
}

#endif //HINAPE_PD_SOLVER_H
