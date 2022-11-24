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
        float damping = 0.99f;
        float time_step = 0.01f;
        float gravity = -9.8f;

        bool distance_constraint = true;
        float distance_constraint_stiffness = 0.7f;
    } _opt;

public:
    void step(real dt) final;

protected:
    void setup_pbd();
    void external_force();
    void time_integration();
    void constraint_projection();
    void update_state();

private:
    std::vector<ConstraintPtr> _constraints;
    float _current_dt;

public:
    explicit PBDSolver(const Opt &opt);
    PBDSolver(const PBDSolver &) = delete;
    PBDSolver(PBDSolver &&) = default;
    ~PBDSolver() = default;
    auto operator=(const PBDSolver &) -> PBDSolver & = delete;
    auto operator=(PBDSolver &&) -> PBDSolver & = default;
};
using PBDSolverPtr = std::shared_ptr<PBDSolver>;
}

#endif //HINAPE_CLOTH_PBD_SOLVER_H
