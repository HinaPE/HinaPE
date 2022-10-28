#include "solver/pbd/pbd_solver.h"

auto main() -> int
{
    auto solver = HinaPE::ClothEngine::PBDSolver::builder().with_stiffness(0).makeShared();

    solver->print_hello();
    return 0;
}
