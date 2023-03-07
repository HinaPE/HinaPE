#include "renderer3D/renderer3D.h"
#include "solver.h"

auto main() -> int
{
	// prepare solver
	auto renderer = std::make_shared<Kasumi::Renderer3D>();
	auto solver = std::make_shared<HinaPE::SPHSolver>();

	// fluid domain
	HinaPE::BoxDomainPtr domain = std::make_shared<HinaPE::BoxDomain>();
	solver->_domain = domain;



	// set up init & step
	renderer->_init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(domain);
		scene->add(solver->_data);
	};

	renderer->_step = [&](real dt)
	{
		solver->update(dt);
	};



	// launch renderer
	renderer->launch();
	return 0;
}