#include "solver.h"

auto main() -> int
{
	auto solver = std::make_shared<HinaPE::RigidSolver>();
	auto sphere = std::make_shared<Kasumi::SphereObject>();
	sphere->POSE.position = {0, 5, 0};
	sphere->_update_surface();
	solver->add(sphere);
	auto box = std::make_shared<Kasumi::CubeObject>();
	box->POSE.euler = {10, 40, 60};
	box->POSE.scale = {1, 0.5, 0.3};
	box->_update_surface();
	solver->add(box, HinaPE::RigidType::Static);

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(sphere);
		scene->add(box);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}