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
		_timer.reset();
		_solver->step(dt);
		_last_update_time = _timer.duration();
	}

	HinaPE::PBDClothSolverPtr _solver;
};

auto main() -> int
{
	std::make_shared<App>()->launch();
	return 0;
}
