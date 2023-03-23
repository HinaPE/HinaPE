#include "renderer3D/renderer3D.h"
#include "smoke_solver.h"

#include "export_to_vol.h"

auto main() -> int
{
	auto solver = std::make_shared<HinaPE::SmokeSolver>();
	solver->init();

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(solver->_data);
		scene->add(solver->_domain);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._key = [&](int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
			solver->reset();
	};

//	save_volume_as_vol(solver->_data->Fluid.density, "test.vol");

	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode();
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}
