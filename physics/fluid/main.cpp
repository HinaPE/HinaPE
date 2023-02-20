#include "backends/api.h"
#include "renderer3D/scene.h"
#include "renderer3D/objects/arrow.h"
#include "lagrangian/solver.h"

class FluidApp : public Kasumi::App
{
protected:
	void prepare() final
	{
		(_solver = std::make_shared<HinaPE::SPHSolver>())
				->sync
						(*(_scene = std::make_shared<Kasumi::Scene3D>()));

		inspect(_scene);

		sync_ui();
	}
	void update(double dt) final
	{
		HINA_TRACK(_solver->step(dt), "SPHFluid")
		_scene->draw();
		sync_ui();
	}

private:
	friend class HinaPE::SPHSolver;
	void sync_ui() {}

private:
	HinaPE::SPHSolverPtr _solver;
	Kasumi::Scene3DPtr _scene;
};

auto main() -> int
{
	std::make_shared<FluidApp>()->launch();
	return 0;
}