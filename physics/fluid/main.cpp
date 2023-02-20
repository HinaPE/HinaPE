#include "backends/api.h"
#include "renderer3D/scene.h"
#include "renderer3D/objects/arrow.h"
#include "lagrangian/solver.h"

class FluidApp : public Kasumi::App
{
protected:
	void prepare() final
	{
		_scene = std::make_shared<Kasumi::Scene3D>();
		_scene->add(_emitter = std::make_shared<Kasumi::ArrowObject>());

		inspect(_scene);
	}
	void update(double dt) final
	{
		sync_ui();
		HINA_TRACK(_solver->step(dt), "SPHFluid");
	}

private:
	friend class HinaPE::SPHSolver;
	void sync_ui()
	{
	}

private:
	HinaPE::SPHSolverPtr _solver;
	Kasumi::Scene3DPtr _scene;
	Kasumi::ArrowObjectPtr _emitter;
};

auto main() -> int
{
	std::make_shared<FluidApp>()->launch();
	return 0;
}