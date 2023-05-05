#include "shader_painter/shader_painter.h"

static std::string painter_frag_src = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
}
)";

class ImageBoard : public Kasumi::ShaderPainter
{
protected:
	void prepare() final;
	void update(double dt) final;

	Kasumi::TexturePtr _texture;
};
void ImageBoard::prepare()
{
	_texture = std::make_shared<Kasumi::Texture>("C:/Users/Administrator/Pictures/_20233917553962.png");
	load_shader(painter_frag_src.c_str());
	resize(_texture->_width, _texture->_height);
	_shader->use();
	_shader->uniform("texture1", 0);

	ShaderPainter::prepare();
}
void ImageBoard::update(double dt)
{
	_texture->bind(0);

	ShaderPainter::update(dt);
}

auto main() -> int
{
	ImageBoard app;
	app.clean_mode();
	app.launch();
	return 0;
}
