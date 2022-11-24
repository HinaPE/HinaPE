#ifndef HINAPE_CLOTH_PBD_SOLVER_H
#define HINAPE_CLOTH_PBD_SOLVER_H

#include "../cloth_solver.h"
#include "constraint.h"
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

        bool distance_constraint = true;
    } _opt;

public:
    void step(real dt) override;

private:
    std::vector<ConstraintPtr> _constraints;

public:
    explicit PBDSolver(const Opt &opt);
    PBDSolver(const PBDSolver &) = delete;
    PBDSolver(PBDSolver &&) = default;
    ~PBDSolver() = default;
    auto operator=(const PBDSolver &) -> PBDSolver & = delete;
    auto operator=(PBDSolver &&) -> PBDSolver & = default;

protected:
    void setup_pbd();
};
using PBDSolverPtr = std::shared_ptr<PBDSolver>;
}

#endif //HINAPE_CLOTH_PBD_SOLVER_H
