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

//	save_volume_as_vol(solver->_data->Fluid.density, "test.vol");

	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode();
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}