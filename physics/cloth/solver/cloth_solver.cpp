#include "cloth_solver.h"

auto HinaPE::ClothEngine::ClothSolver::builder() -> HinaPE::ClothEngine::ClothSolver::Builder { return {}; }
HinaPE::ClothEngine::ClothSolver::ClothSolver(const HinaPE::ClothEngine::ClothGeometryDataPtr &cloth_geometry_data_ptr) { _cloth_geometry_data = cloth_geometry_data_ptr; }
void HinaPE::ClothEngine::ClothSolver::on_begin_advance_time_step(double time_interval_in_seconds) {}
void HinaPE::ClothEngine::ClothSolver::on_end_advance_time_step(double time_interval_in_seconds) {}
auto HinaPE::ClothEngine::ClothSolver::Builder::build() const -> HinaPE::ClothEngine::ClothSolver { return ClothSolver(std::make_shared<ClothGeometryData>(_width, _height, _cols, _rows)); }
auto HinaPE::ClothEngine::ClothSolver::Builder::make_shared() const -> HinaPE::ClothEngine::ClothSolverPtr { return {new ClothSolver(std::make_shared<ClothGeometryData>(_width, _height, _cols, _rows)), [](ClothSolver *obj) { delete obj; }}; }

void HinaPE::ClothEngine::ClothSolver::onInitialize() { PhysicsAnimation::onInitialize(); }
void HinaPE::ClothEngine::ClothSolver::onAdvanceTimeStep(double timeIntervalInSeconds)
{
    on_begin_advance_time_step(timeIntervalInSeconds);

    on_end_advance_time_step(timeIntervalInSeconds);
}
