#include "renderer3D/renderer3D.h"

struct Sakura : public Kasumi::ObjectMesh3D
{
	Sakura()
	{
		NAME = "Sakura";
		_shader = Kasumi::Shader::DefaultMeshShader;
		_init("sakura", "");
		set_color(HinaPE::Color::SAKURA);
		_switch_wireframe();
	}
};

auto main() -> int
{
	auto sakura = std::make_shared<Sakura>();
	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene) { scene->add(sakura); };
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}