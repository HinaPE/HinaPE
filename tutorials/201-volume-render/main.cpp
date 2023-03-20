#include "shader_painter/shader_painter.h"

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(VolumeRender);

auto main() -> int
{
	auto fs = cmrc::VolumeRender::get_filesystem();
	auto cloud = fs.open("shaders/cloud.glsl").begin();

	auto painter = std::make_shared<Kasumi::ShaderPainter>();
	painter->clean_mode();
	painter->close_menu();
	painter->load_shader(cloud);
	painter->launch();
	return 0;
}
