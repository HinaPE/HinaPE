#include "cloth_solver.h"

auto HinaPE::Cloth::ClothSolver::builder() -> HinaPE::Cloth::ClothSolver::Builder { return {}; }
HinaPE::Cloth::ClothSolver::ClothSolver(const HinaPE::Cloth::ClothGeometryDataPtr &cloth_geometry_data_ptr) { _cloth_geometry_data = cloth_geometry_data_ptr; }
void HinaPE::Cloth::ClothSolver::on_begin_advance_time_step(double time_interval_in_seconds) {}
void HinaPE::Cloth::ClothSolver::on_end_advance_time_step(double time_interval_in_seconds) {}
auto HinaPE::Cloth::ClothSolver::Builder::build() const -> HinaPE::Cloth::ClothSolver { return ClothSolver(std::make_shared<ClothGeometryData>(_width, _height, _cols, _rows)); }
auto HinaPE::Cloth::ClothSolver::Builder::make_shared() const -> HinaPE::Cloth::ClothSolverPtr { return {new ClothSolver(std::make_shared<ClothGeometryData>(_width, _height, _cols, _rows)), [](ClothSolver *obj) { delete obj; }}; }

void HinaPE::Cloth::ClothSolver::onInitialize() { PhysicsAnimation::onInitialize(); }
void HinaPE::Cloth::ClothSolver::onAdvanceTimeStep(double timeIntervalInSeconds)
{
    on_begin_advance_time_step(timeIntervalInSeconds);

    on_end_advance_time_step(timeIntervalInSeconds);
}
