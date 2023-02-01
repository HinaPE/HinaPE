#include "grid_boundary_condition_solver.h"
void Hina::GridFractionalBoundaryConditionSolver3::constrain_velocity(Hina::FaceCenteredVectorGrid3Ptr &ptr, unsigned int extrapolation_depth)
{
}
auto Hina::GridFractionalBoundaryConditionSolver3::collider_sdf() const -> Hina::ScalarField3Ptr
{
	return Hina::ScalarField3Ptr();
}
auto Hina::GridFractionalBoundaryConditionSolver3::collider_velocity_field() const -> Hina::VectorField3Ptr
{
	return Hina::VectorField3Ptr();
}
void Hina::GridFractionalBoundaryConditionSolver3::on_collider_updated(const Hina::Base::Size3 &grid_size, const Hina::mVector3 &grid_spacing, const Hina::mVector3 &grid_origin)
{
}
