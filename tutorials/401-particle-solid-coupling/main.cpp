#include "renderer3D/renderer3D.h"
#include "rigid/solver.h"
#include "fluid-particles/sph_solver.h"

auto main() -> int
{
	auto rigid = std::make_shared<HinaPE::RigidSolver>();
	auto fluid = std::make_shared<HinaPE::SPHSolver>();

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
//		rigid->update(dt);
//		fluid->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._debugger = [&]() {};

	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(rigid.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(fluid.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}