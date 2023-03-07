#include "renderer3D/renderer3D.h"
#include "solver.h"

void setup_renderer(const Kasumi::Renderer3DPtr &renderer, const HinaPE::SPHSolverPtr &solver)
{
	renderer->_init = [&](const Kasumi::Scene3DPtr &scene)
	{
		HinaPE::BoxDomainPtr domain = std::make_shared<HinaPE::BoxDomain>();
		scene->add(domain);
	};
	renderer->_step = [&](real dt)
	{
	};
}

auto main() -> int
{
	auto renderer = std::make_shared<Kasumi::Renderer3D>();
	auto solver = std::make_shared<HinaPE::SPHSolver>();
	setup_renderer(renderer, solver);
	renderer->launch();
	return 0;
}