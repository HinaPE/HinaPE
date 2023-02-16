#include "app.h"

#include "pbd/solver.h"

class App : public Kasumi::App
{
protected:
	void prepare() final
	{
		_solver = std::make_shared<HinaPE::PBDClothSolver>();
	}
	void update(double dt) final
	{
		timer.reset();
		_solver->step(dt);
		_last_update_time = timer.duration();
	}

	HinaPE::PBDClothSolverPtr _solver;
	HinaPE::Util::Timer timer;
};

auto main() -> int
{
	std::make_shared<App>()->launch();
	return 0;
}