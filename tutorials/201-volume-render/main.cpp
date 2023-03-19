#include "shader_painter/shader_painter.h"

auto main() -> int
{
	auto painter = std::make_shared<Kasumi::ShaderPainter>();
	painter->load_shader(std::string(MyShaderDir) + "bunny.glsl");
	painter->launch();
	return 0;
}
