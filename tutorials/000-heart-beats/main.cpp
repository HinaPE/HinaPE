#include "shader_painter/shader_painter.h"

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(HeartBeats);

auto main() -> int
{
	auto fs = cmrc::HeartBeats::get_filesystem();
	auto heart = fs.open("shaders/heart.glsl").begin();

	auto painter = std::make_shared<Kasumi::ShaderPainter>();
	painter->close_menu();
	painter->clean_mode();
	painter->load_shader(heart);
	painter->launch();
	return 0;
}
