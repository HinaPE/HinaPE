#include "renderer3D/renderer3D.h"
#include "cloud_solver.h"
#include "export.h"

auto main() -> int
{
	auto solver = std::make_shared<CloudSolver>();
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
		if (key == GLFW_KEY_O && action == GLFW_PRESS)
			save_volume_as_tga(solver->_data->Cloud.final, "test.tga");
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode();
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}