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
public:
	void load_image(const std::string &path)
	{
		_texture = std::make_shared<Kasumi::Texture>(path);
		auto p = _texture->get(0, 0);
		mVector4 s(0, 0, 0, 1);
		for (int i = 0; i < 100; ++i)
			for (int j = 0; j < 100; ++j)
				_texture->set(i, j, s);
		_texture->update();
	}

protected:
	void prepare() final
	{
		load_shader(painter_frag_src.c_str());
		resize(_texture->_width, _texture->_height);
		_shader->use();
		_shader->uniform("texture1", 0);
		ShaderPainter::prepare();
	}
	void update(double dt) final
	{
		_texture->bind(0);
		ShaderPainter::update(dt);
	}

	Kasumi::TexturePtr _texture;
};

auto main(int argc, char **argv) -> int
{
	ImageBoard app;
	if (argc > 1)
	{
		auto path = std::string(argv[1]);
		app.load_image(path);
	} else
	{
		app.load_image("C:/Users/Administrator/Pictures/_20233917553962.png");
	}

	app.clean_mode();
	app.launch();
	return 0;
}
