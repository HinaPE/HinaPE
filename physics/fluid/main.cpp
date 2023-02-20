#include "backends/api.h"
#include "lagrangian/solver.h"

class FluidApp : public Kasumi::App
{
protected:
	void prepare() final
	{
		inspect(_solver = std::make_shared<HinaPE::SPHSolver>());
	}
	void update(double dt) final
	{
		HINA_TRACK(_solver->step(dt), "SPHFluid");
	}

private:
	HinaPE::SPHSolverPtr _solver;
};

auto main() -> int
{
	std::make_shared<FluidApp>()->launch();
	return 0;
}