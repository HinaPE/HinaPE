#include "grid_boundary_condition_solver.h"
void HinaPE::GridFractionalBoundaryConditionSolver3::constrain_velocity(HinaPE::Geom::FaceCenteredVectorGrid3Ptr &ptr, unsigned int extrapolation_depth)
{
}
auto HinaPE::GridFractionalBoundaryConditionSolver3::collider_sdf() const -> HinaPE::Geom::ScalarField3Ptr
{
	return {};
}
auto HinaPE::GridFractionalBoundaryConditionSolver3::collider_velocity_field() const -> HinaPE::Geom::VectorField3Ptr
{
	return {};
}
void HinaPE::GridFractionalBoundaryConditionSolver3::on_collider_updated(const mSize3 &grid_size, const mVector3 &grid_spacing, const mVector3 &grid_origin)
{
}
