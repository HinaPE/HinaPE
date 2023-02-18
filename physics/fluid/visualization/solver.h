#ifndef HINAPE_VIS_SOLVER_H
#define HINAPE_VIS_SOLVER_H

#include "backends/api.h"
#include "lagrangian/solver.h"

namespace HinaPE
{
class VisSPHSolver : public SPHSolver, public Kasumi::INSPECTOR
{
public:
	void INSPECT() final
	{
		INSPECT_VEC3(_opt.gravity, "gravity");
		INSPECT_REAL(_opt.eos_exponent, "eos exponent");
		INSPECT_REAL(_opt.negative_pressure_scale, "negative pressure scale");
		INSPECT_REAL(_opt.viscosity_coefficient, "viscosity coefficient");
		INSPECT_REAL(_opt.pseudo_viscosity_coefficient, "pseudo viscosity coefficient");
		INSPECT_REAL(_opt.speed_of_sound, "speed of sound");
		INSPECT_REAL(_opt.time_step_limit_scale, "time step limit scale");
	}
};
using VisSPHSolverPtr = std::shared_ptr<VisSPHSolver>;
}

#endif //HINAPE_VIS_SOLVER_H
