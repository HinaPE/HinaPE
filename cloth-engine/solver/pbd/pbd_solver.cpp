#include "pbd_solver.h"

auto HinaPE::ClothEngine::PBDSolver::builder() -> HinaPE::ClothEngine::PBDSolver::Builder { return {}; }
auto HinaPE::ClothEngine::PBDSolver::Builder::build() const -> HinaPE::ClothEngine::PBDSolver { return {}; }
auto HinaPE::ClothEngine::PBDSolver::Builder::makeShared() const -> HinaPE::ClothEngine::PBDSolverPtr { return {new PBDSolver(), [](PBDSolver *obj) { delete obj; }}; }
void HinaPE::ClothEngine::PBDSolver::update(double dt) {}
