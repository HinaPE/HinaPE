#include "renderer3D/renderer3D.h"
#include "pbd/solver.h"

auto main() -> int
{
	// prepare solver
	auto solver = std::make_shared<HinaPE::PBDClothSolver>();

	// set up init & step
	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(solver->_data);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(solver.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}
