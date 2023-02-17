#ifndef HINAPE_GRID_DIFFUSION_SOLVER_H
#define HINAPE_GRID_DIFFUSION_SOLVER_H

#include "geom/grid.h"

namespace HinaPE
{
class GridDiffusionSolver3
{
};
class GridForwardEulerDiffusionSolver3 final : public GridDiffusionSolver3 {};
class GridBackwardEulerDiffusionSolver3 final : public GridDiffusionSolver3 {};

using GridDiffusionSolver3Ptr = std::shared_ptr<GridDiffusionSolver3>;
using GridForwardEulerDiffusionSolver3Ptr = std::shared_ptr<GridForwardEulerDiffusionSolver3>;
using GridBackwardEulerDiffusionSolver3Ptr = std::shared_ptr<GridBackwardEulerDiffusionSolver3>;
}
#endif //HINAPE_GRID_DIFFUSION_SOLVER_H
