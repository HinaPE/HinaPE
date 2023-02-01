#ifndef HINAPE_GRID_PRESSURE_SOLVER_H
#define HINAPE_GRID_PRESSURE_SOLVER_H

#include "base/base.h"

#include "grid.h"
#include "grid_boundary_condition_solver.h"

namespace Hina
{
class GridPressureSolver3
{
public:
	virtual void solve(const FaceCenteredVectorGrid3Ptr &input, real dt, FaceCenteredVectorGrid3Ptr &output, const ScalarField3 &boundary_sdf, const VectorField3 &boundary_velocity, const ScalarField3 &fluid_sdf, bool use_compressed) = 0;
	virtual auto suggest_boundary_condition_solver() const -> GridBoundaryConditionSolver3Ptr = 0;
};
class GridSinglePhasePressureSolver3 : public GridPressureSolver3
{
public:
	void solve(const FaceCenteredVectorGrid3Ptr &input, real dt, FaceCenteredVectorGrid3Ptr &output, const ScalarField3 &boundary_sdf, const VectorField3 &boundary_velocity, const ScalarField3 &fluid_sdf, bool use_compressed) override;
	auto suggest_boundary_condition_solver() const -> GridBoundaryConditionSolver3Ptr override;
};
class GridFractionalSinglePhasePressureSolver3 : public GridPressureSolver3
{
public:
	void solve(const FaceCenteredVectorGrid3Ptr &input, real dt, FaceCenteredVectorGrid3Ptr &output, const ScalarField3 &boundary_sdf, const VectorField3 &boundary_velocity, const ScalarField3 &fluid_sdf, bool use_compressed) override;
	auto suggest_boundary_condition_solver() const -> GridBoundaryConditionSolver3Ptr override;
};

using GridPressureSolver3Ptr = std::shared_ptr<GridPressureSolver3>;
using GridSinglePhasePressureSolver3Ptr = std::shared_ptr<GridSinglePhasePressureSolver3>;
using GridFractionalSinglePhasePressureSolver3Ptr = std::shared_ptr<GridFractionalSinglePhasePressureSolver3>;
}
#endif //HINAPE_GRID_PRESSURE_SOLVER_H
