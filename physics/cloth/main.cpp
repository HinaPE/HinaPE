#include "renderer3D/renderer3D.h"
#include "pbd/solver.h"

auto main() -> int
{
	// prepare solver
	auto renderer = std::make_shared<Kasumi::Renderer3D>();
	auto solver = std::make_shared<HinaPE::PBDClothSolver>();

	// set up init & step
	renderer->_init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(solver->_data);
	};

	renderer->_step = [&](real dt)
	{
		solver->update(dt);
	};

	renderer->inspect(solver.get());

	// launch renderer
	renderer->launch();
	return 0;
}
