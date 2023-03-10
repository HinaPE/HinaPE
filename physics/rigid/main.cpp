#include "solver.h"

auto main() -> int
{
	auto solver = std::make_shared<HinaPE::RigidSolver>();
	auto sphere1 = std::make_shared<Kasumi::SphereObject>();
	solver->add(sphere1, HinaPE::RigidType::Static);
	auto sphere2 = std::make_shared<Kasumi::SphereObject>();
	sphere2->POSE.position = {0, 5, 0};
	solver->add(sphere2);

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(sphere1);
		scene->add(sphere2);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}