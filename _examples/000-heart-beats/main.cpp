#include "glad/glad.h"

#include "app.h"
#include "shader.h"
#include "framebuffer.h"
#include "objects/object3D.h"

#include <chrono>

class Painter final : public Kasumi::App
{
public:
	Painter() : Kasumi::App() {}
	void prepare() final
	{
		Kasumi::Shader::Init();
		_shader = std::make_shared<Kasumi::Shader>(std::string(MyShaderDir) + "painter_vertex.glsl", std::string(MyShaderDir) + "heart.glsl");
		_framebuffer = std::make_shared<Kasumi::Framebuffer>(_opt.width, _opt.height);

		// draw a rectangle to fill the screen
		std::array<float, 24> screen_vertices = {
				-1.0, -1.0, 0.0, 0.0,
				-1.0, 1.0, 0.0, 1.0,
				1.0, -1.0, 1.0, 0.0,

				1.0, -1.0, 1.0, 0.0,
				-1.0, 1.0, 0.0, 1.0,
				1.0, 1.0, 1.0, 1.0
		};
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &screen_vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		_framebuffer->render_callback = [&]() // render the scene to the framebuffer
		{
			_shader->use();
			static mVector2 screen(_opt.width, _opt.height);
			_shader->uniform("iResolution", screen);
			static std::chrono::steady_clock::time_point _starting_point = std::chrono::steady_clock::now();
			float time = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _starting_point).count()) / 1000000.f;
			_shader->uniform("iTime", time);
			glBindVertexArray(_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		};
	}
	void update(double dt) final
	{
		_framebuffer->render();
	}

private:
	Kasumi::ShaderPtr _shader;
	Kasumi::FramebufferPtr _framebuffer;

	unsigned int _vao{0};
};

auto main() -> int
{
	auto painter = std::make_shared<Painter>();
	painter->launch();
	return 0;
}