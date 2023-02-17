#include "grid_pressure_solver.h"
void HinaPE::GridSinglePhasePressureSolver3::solve(const HinaPE::Geom::FaceCenteredVectorGrid3Ptr &input, real dt, HinaPE::Geom::FaceCenteredVectorGrid3Ptr &output, const HinaPE::Geom::ScalarField3 &boundary_sdf, const HinaPE::Geom::VectorField3 &boundary_velocity, const HinaPE::Geom::ScalarField3 &fluid_sdf, bool use_compressed)
{
}
auto HinaPE::GridSinglePhasePressureSolver3::suggest_boundary_condition_solver() const -> HinaPE::GridBoundaryConditionSolver3Ptr
{
	return {};
}
void HinaPE::GridFractionalSinglePhasePressureSolver3::solve(const HinaPE::Geom::FaceCenteredVectorGrid3Ptr &input, real dt, HinaPE::Geom::FaceCenteredVectorGrid3Ptr &output, const HinaPE::Geom::ScalarField3 &boundary_sdf, const HinaPE::Geom::VectorField3 &boundary_velocity, const HinaPE::Geom::ScalarField3 &fluid_sdf, bool use_compressed)
{
}
auto HinaPE::GridFractionalSinglePhasePressureSolver3::suggest_boundary_condition_solver() const -> HinaPE::GridBoundaryConditionSolver3Ptr
{
	return {};
}
