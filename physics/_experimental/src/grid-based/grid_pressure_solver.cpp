#include "grid_pressure_solver.h"
void Hina::GridSinglePhasePressureSolver3::solve(const Hina::FaceCenteredVectorGrid3Ptr &input, real dt, Hina::FaceCenteredVectorGrid3Ptr &output, const Hina::ScalarField3 &boundary_sdf, const Hina::VectorField3 &boundary_velocity, const Hina::ScalarField3 &fluid_sdf, bool use_compressed)
{
}
auto Hina::GridSinglePhasePressureSolver3::suggest_boundary_condition_solver() const -> Hina::GridBoundaryConditionSolver3Ptr
{
	return {};
}
void Hina::GridFractionalSinglePhasePressureSolver3::solve(const Hina::FaceCenteredVectorGrid3Ptr &input, real dt, Hina::FaceCenteredVectorGrid3Ptr &output, const Hina::ScalarField3 &boundary_sdf, const Hina::VectorField3 &boundary_velocity, const Hina::ScalarField3 &fluid_sdf, bool use_compressed)
{
}
auto Hina::GridFractionalSinglePhasePressureSolver3::suggest_boundary_condition_solver() const -> Hina::GridBoundaryConditionSolver3Ptr
{
	return {};
}
