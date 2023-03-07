#include "renderer3D/renderer3D.h"
#include "sph_solver.h"

auto main() -> int
{
	// prepare solver
	auto renderer = std::make_shared<Kasumi::Renderer3D>();
	auto solver = std::make_shared<HinaPE::SPHSolver>();

	// solver init
	auto data = std::make_shared<HinaPE::SPHSolver::Data>();
	auto domain = std::make_shared<HinaPE::BoxDomain>();
	auto emitter = std::make_shared<HinaPE::PointParticleEmitter3>();
	solver->_data = data;
	solver->_domain = domain;
	solver->_emitter = emitter;


	// set up init & step
	renderer->_init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(data);
		scene->add(domain);
		scene->add(emitter);
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