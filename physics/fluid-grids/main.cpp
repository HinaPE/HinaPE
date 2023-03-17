#include "renderer3D/renderer3D.h"
#include "smoke_solver.h"

auto main() -> int
{
	auto solver = std::make_shared<HinaPE::SmokeSolver>();
	auto data = std::make_shared<HinaPE::SmokeSolver::Data>();
	auto domain = std::make_shared<HinaPE::BoxDomain>();

	solver->_data = data;

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(data);
		scene->add(domain);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode();
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}