#ifndef HINAPE_GRID_BOUNDARY_CONDITION_SOLVER_H
#define HINAPE_GRID_BOUNDARY_CONDITION_SOLVER_H

#include "geom/grid.h"
#include "geom/collider3.h"

namespace HinaPE
{
// @formatter:off
class GridBoundaryConditionSolver3
{
public:
	void update_collider(const Geom::ColliderPtr &collider, const mSize3 &grid_size, const mVector3 &grid_spacing, const mVector3 &grid_origin) { _opt.collider = collider; _opt.grid_size = grid_size; _opt.grid_spacing = grid_spacing; _opt.grid_origin = grid_origin;}
	virtual void constrain_velocity(Geom::FaceCenteredVectorGrid3Ptr &ptr, unsigned int extrapolation_depth) = 0; // extrapolation_depth = 5 by default
	virtual auto collider_sdf() const -> Geom::ScalarField3Ptr = 0;
	virtual auto collider_velocity_field() const -> Geom::VectorField3Ptr = 0;

public:
	struct Opt
	{
		Geom::ColliderPtr collider;
		mSize3 grid_size;
		mVector3 grid_spacing;
		mVector3 grid_origin;
		int closed_domain_boundary_flag = Direction::DirectionAll;
	} _opt;

protected:
	virtual void on_collider_updated(const mSize3 &grid_size, const mVector3 &grid_spacing, const mVector3 &grid_origin) = 0;
};
// @formatter:on
class GridFractionalBoundaryConditionSolver3 : public GridBoundaryConditionSolver3
{
public:
	void constrain_velocity(Geom::FaceCenteredVectorGrid3Ptr &ptr, unsigned int extrapolation_depth) override;
	auto collider_sdf() const -> Geom::ScalarField3Ptr override;
	auto collider_velocity_field() const -> Geom::VectorField3Ptr override;
protected:
	void on_collider_updated(const mSize3 &grid_size, const mVector3 &grid_spacing, const mVector3 &grid_origin) override;
};
class GridBlockedBoundaryConditionSolver3 final : public GridFractionalBoundaryConditionSolver3
{
};
using GridBoundaryConditionSolver3Ptr = std::shared_ptr<GridBoundaryConditionSolver3>;
using GridFractionalBoundaryConditionSolver3Ptr = std::shared_ptr<GridFractionalBoundaryConditionSolver3>;
using GridBlockedBoundaryConditionSolver3Ptr = std::shared_ptr<GridBlockedBoundaryConditionSolver3>;
}
#endif //HINAPE_GRID_BOUNDARY_CONDITION_SOLVER_H
