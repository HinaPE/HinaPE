#include "renderer3D/renderer3D.h"
#include "domain/box_domain.h"

auto main() -> int
{
	auto domain = std::make_shared<HinaPE::GridBoxDomain>();

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(domain);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode();
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}