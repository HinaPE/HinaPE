#include "solver.h"

Hina::GridFluidSolver::GridFluidSolver()
{
	_grids = std::make_shared<GridSystemData3>();
}
void Hina::GridFluidSolver::on_physics_init()
{
}
void Hina::GridFluidSolver::on_physics_update(float time_step)
{
}
