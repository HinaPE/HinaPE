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

		inspect(_solver);

		sync_ui();
	}
	void update(double dt) final
	{
		HINA_TRACK(_solver->step(dt), "SPHFluid")
		_scene->draw();
		sync_ui();
	}
	void key(int key, int scancode, int action, int mods) final { _scene->key(key, scancode, action, mods); }
	void mouse_button(int button, int action, int mods) final { _scene->mouse_button(button, action, mods); }
	void mouse_scroll(double x_offset, double y_offset) final { _scene->mouse_scroll(x_offset, y_offset); }
	void mouse_cursor(double x_pos, double y_pos) final { _scene->mouse_cursor(x_pos, y_pos); }

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