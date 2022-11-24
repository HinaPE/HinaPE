#ifndef HINAPE_CLOTH_PBD_SOLVER_H
#define HINAPE_CLOTH_PBD_SOLVER_H

#include "../cloth_solver.h"
#include <memory>

namespace HinaPE::Cloth
{
class PBDSolver : public ClothSolver
{
public:
    struct Opt : ClothSolver::Opt
    {
        float stiffness = 1.f;
        float damping = 0.1f;
        float time_step = 0.01f;
        float gravity = -9.8f;
    } _opt;

public:
    explicit PBDSolver(const Opt &opt);
    PBDSolver(const PBDSolver &) = delete;
    PBDSolver(PBDSolver &&) = default;
    ~PBDSolver() = default;
    auto operator=(const PBDSolver &) -> PBDSolver & = delete;
    auto operator=(PBDSolver &&) -> PBDSolver & = default;

protected:
    void setup();
};
using PBDSolverPtr = std::shared_ptr<PBDSolver>;
}

#endif //HINAPE_CLOTH_PBD_SOLVER_H
