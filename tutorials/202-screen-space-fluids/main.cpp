#include "renderer3D/renderer3D.h"
#include "simple_fluid.h"
#include "ssf_renderer.h"

auto main() -> int
{
	HinaPE::SimpleFluid fluid;
	HinaPE::SSFRenderer ssf;

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		ssf.init_shaders();
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{

	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode(); // we use dark mode app to protect our eyes~ XD
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}