#include "renderer3D/renderer3D.h"
#include "fluid-particles/pbf_solver_new.h"

using SolverType = HinaPE::PBFSolverNew;
using SolverDataType = SolverType::Data;

auto main() -> int
{
	auto solver = std::make_shared<SolverType>();

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		solver->init();
		scene->add(solver->_data);
		scene->add(solver->_domain);
		scene->_scene_opt._particle_mode = true;
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(solver.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode(); // we use dark mode app to protect our eyes~ XD
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}