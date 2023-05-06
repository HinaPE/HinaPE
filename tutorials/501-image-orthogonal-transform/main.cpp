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

auto operator+(mVector3 v, Kasumi::Texture::Pixel p) -> mVector3
{
	return v + mVector3(p.x(), p.y(), p.z());
}

auto operator+(Kasumi::Texture::Pixel p, mVector3 v) -> mVector3
{
	return v + mVector3(p.x(), p.y(), p.z());
}

auto operator+=(mVector3 &v, Kasumi::Texture::Pixel p) -> mVector3 &
{
	v = v + p;
	return v;
}

void filter9x9(Kasumi::TexturePtr ptr)
{
	auto &image = *ptr;

	auto width = image._width;
	auto height = image._height;

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			mVector3 temp;

			temp += image(i, j);
			if (i - 1 > 0 && j - 1 > 0)
				temp += image(i - 1, j - 1);
			else
				temp += image(i, j);

			if (i - 1 > 0)
				temp += image(i - 1, j);
			else
				temp += image(i, j);

			if (i - 1 > 0 && j + 1 < height)
				temp += image(i - 1, j + 1);
			else
				temp += image(i, j);

			if (j - 1 > 0)
				temp += image(i, j - 1);
			else
				temp += image(i, j);

			if (j + 1 < height)
				temp += image(i, j + 1);
			else
				temp += image(i, j);

			if (i + 1 < width && j - 1 > 0)
				temp += image(i + 1, j - 1);
			else
				temp += image(i, j);

			if (i + 1 < width)
				temp += image(i + 1, j);
			else
				temp += image(i, j);

			if (i + 1 < width && j + 1 < height)
				temp += image(i + 1, j + 1);
			else
				temp += image(i, j);

			temp /= 29.0f;

			image(i, j) = Kasumi::Texture::Pixel(temp.x(), temp.y(), temp.z());
		}
	}

	image.update();
}

class ImageBoard : public Kasumi::ShaderPainter
{
public:
	void load_image(const std::string &path)
	{
		_texture = std::make_shared<Kasumi::Texture>(path);
		filter9x9(_texture);
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
		app.load_image("len_std.jpg");
	}

	app.clean_mode();
	app.launch();
	return 0;
}
