#include "solver.h"

Hina::GridFluidSolver::GridFluidSolver(const Hina::Base::Size3 &resolution, const Hina::mVector3 &grid_spacing, const Hina::mVector3 &origin)
{
	_grids = std::make_shared<GridSystemData3>();
	_grids->resize(resolution, grid_spacing, origin);
}
void Hina::GridFluidSolver::on_physics_init()
{
}
void Hina::GridFluidSolver::on_physics_update(float time_step)
{
}
