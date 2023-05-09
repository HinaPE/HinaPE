#include "renderer3D/renderer3D.h"

auto main() -> int
{
	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		auto bunny = std::make_shared<Kasumi::BunnyObject>();
		bunny->set_color(HinaPE::Color::WHITE);
		scene->add(bunny);
	};
	Kasumi::Renderer3D::DEFAULT_RENDERER.close_benchmark();
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}
