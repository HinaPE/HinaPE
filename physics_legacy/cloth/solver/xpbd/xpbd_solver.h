#ifndef HINAPE_XPBD_SOLVER_H
#define HINAPE_XPBD_SOLVER_H

#include "../cloth_solver.h"
#include <memory>

namespace HinaPE::Cloth
{
class XPBDSolver : public ClothSolver
{
public:
    void step(real dt) final;
};
using XPBDSolverPtr = std::shared_ptr<XPBDSolver>;
}

#endif //HINAPE_XPBD_SOLVER_H
