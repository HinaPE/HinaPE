#ifndef HINAPE_FMS_SOLVER_H
#define HINAPE_FMS_SOLVER_H

#include "../cloth_solver.h"
#include <memory>

namespace HinaPE::Cloth
{
class FMSSolver : public ClothSolver
{
public:
    void step(real dt) final;
};
using FMSSolverPtr = std::shared_ptr<FMSSolver>;
}

#endif //HINAPE_FMS_SOLVER_H
