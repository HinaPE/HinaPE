#include "renderer3D/renderer3D.h"
#include "rigid/solver.h"
#include "fluid-particles/sph_solver.h"

auto main() -> int
{
	auto renderer = std::make_shared<Kasumi::Renderer3D>();
	auto rigid = std::make_shared<HinaPE::RigidSolver>();
	auto fluid = std::make_shared<HinaPE::SPHSolver>();

	renderer->_init = [&](const Kasumi::Scene3DPtr &scene)
	{
	};

	renderer->_step = [&](real dt)
	{
		rigid->update(dt);
//		fluid->update(dt);
	};

	renderer->_debugger = [&]()
	{
	};

	renderer->inspect(rigid.get());
	renderer->inspect(fluid.get());

	renderer->launch();
	return 0;
}