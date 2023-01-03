#include "glad/glad.h"

#include "backends/platform.h"
#include "backends/shader.h"
#include "backends/framebuffer.h"

#include <chrono>

class Painter final : public Kasumi::App
{
public:
	Painter() : Kasumi::App(800, 600, "2D Painter") {}
	void prepare() final
	{
		_shader1 = std::make_shared<Kasumi::Shader>(std::string(MyShaderDir) + "painter_vertex.glsl", std::string(MyShaderDir) + "heart.glsl");
		_shader2 = std::make_shared<Kasumi::Shader>(std::string(MyShaderDir) + "painter_vertex.glsl", std::string(MyShaderDir) + "cloud.glsl");
		_shader3 = std::make_shared<Kasumi::Shader>(std::string(MyShaderDir) + "painter_vertex.glsl", std::string(MyShaderDir) + "field_visualizer.glsl");
		_shader4 = std::make_shared<Kasumi::Shader>(std::string(MyShaderDir) + "painter_vertex.glsl", std::string(MyShaderDir) + "painter_fragment.glsl");
		_framebuffer1 = std::make_shared<Kasumi::Framebuffer>(_width, _height, -1, 0, 0, 1); // left top frame
		_framebuffer2 = std::make_shared<Kasumi::Framebuffer>(_width, _height, 0, 0, 1, 1); // right top frame
		_framebuffer3 = std::make_shared<Kasumi::Framebuffer>(_width, _height, -1, -1, 0, 0); // left bottom frame
		_framebuffer4 = std::make_shared<Kasumi::Framebuffer>(_width, _height, 0, -1, 1, 0); // right bottom frame

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

		_framebuffer1->render_callback = [&]() // render the scene to the framebuffer
		{
			_shader1->use();
			static mVector2 screen(_width, _height);
			_shader1->uniform("iResolution", screen);
			static std::chrono::steady_clock::time_point _starting_point = std::chrono::steady_clock::now();
			float time = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _starting_point).count()) / 1000000.f;
			_shader1->uniform("iTime", time);
			glBindVertexArray(_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		};
		_framebuffer2->render_callback = [&]() // render the scene to the framebuffer
		{
			_shader2->use();
			static mVector2 screen(_width, _height);
			_shader2->uniform("iResolution", screen);
			static std::chrono::steady_clock::time_point _starting_point = std::chrono::steady_clock::now();
			float time = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _starting_point).count()) / 1000000.f;
			_shader2->uniform("iTime", time);
			glBindVertexArray(_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		};
		_framebuffer3->render_callback = [&]() // render the scene to the framebuffer
		{
			_shader3->use();
			static mVector2 screen(_width, _height);
			_shader3->uniform("iResolution", screen);
			static std::chrono::steady_clock::time_point _starting_point = std::chrono::steady_clock::now();
			float time = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _starting_point).count()) / 1000000.f;
			_shader3->uniform("iTime", time);
			glBindVertexArray(_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		};
		_framebuffer4->render_callback = [&]() // render the scene to the framebuffer
		{
			_shader4->use();
			static mVector2 screen(_width, _height);
			_shader4->uniform("iResolution", screen);
			static std::chrono::steady_clock::time_point _starting_point = std::chrono::steady_clock::now();
			float time = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _starting_point).count()) / 1000000.f;
			_shader4->uniform("iTime", time);
			glBindVertexArray(_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		};
	}
	void update(double dt) final
	{
		_framebuffer1->render();
		_framebuffer2->render();
		_framebuffer3->render();
		_framebuffer4->render();
	}

private:
	Kasumi::ShaderPtr _shader1, _shader2, _shader3, _shader4;
	Kasumi::FramebufferPtr _framebuffer1, _framebuffer2, _framebuffer3, _framebuffer4;

	unsigned int _vao{0};
};

auto main() -> int
{
	std::make_shared<Painter>()->launch();
	return 0;
}