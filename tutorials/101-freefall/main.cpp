#include "renderer3D/renderer3D.h"

class SimpleFreeFallSystem
{
public:
	void add_sphere() {}
	void add_box() {}
	void add_plane() {}
	void update(real dt) {}
};

auto main() -> int
{
	auto free_fall = std::make_shared<SimpleFreeFallSystem>();
	auto renderer = std::make_shared<Kasumi::Renderer3D>();
	renderer->_init = [&](const Kasumi::Scene3DPtr &scene)
	{
		auto sphere = std::make_shared<Kasumi::SphereObject>();
		sphere->POSE.position = {0, 5, 0};
		scene->add(sphere);
		auto cube = std::make_shared<Kasumi::CubeObject>();
		cube->POSE.position = {0, 0, 0};
		scene->add(cube);
	};
	renderer->_step = [&](real dt)
	{
	};
	renderer->launch();
	return 0;
}