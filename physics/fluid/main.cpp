#include "backends/app.h"
#include "visualization/solver.h"

class FluidApp : public Kasumi::App
{
protected:
	void prepare() final
	{
		_solver = std::make_shared<HinaPE::VisSPHSolver>();
		inspect(_solver);
	}
	void update(double dt) final
	{
		_solver->step(dt);
	}

private:
	HinaPE::VisSPHSolverPtr _solver;
};

auto main() -> int
{
	std::make_shared<FluidApp>()->launch();
	return 0;
}