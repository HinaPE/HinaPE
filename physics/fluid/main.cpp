#include "app.h"
#include "lagrangian/solver.h"

class FluidApp : public Kasumi::App
{
protected:
	void prepare() final
	{
		_solver = std::make_shared<HinaPE::SPHSolver>();
	}
	void update(double dt) final
	{
		_solver->step(dt);
	}

private:
	HinaPE::SPHSolverPtr _solver;
};

auto main() -> int
{
	std::make_shared<FluidApp>()->launch();
	return 0;
}