#include "glad/glad.h"

#include "backends/platform.h"
#include "backends/shader.h"
#include "backends/framebuffer.h"

class Painter : public Kasumi::App
{
public:
	Painter() : Kasumi::App(800, 600, "2D Painter"), _vao(0) {}
	void prepare() final
	{
//		_shader = std::make_shared<Kasumi::Shader>(std::string(MyShaderDir) + "painter_vertex.glsl", std::string(MyShaderDir) + "painter_fragment.glsl");
		_shader = std::make_shared<Kasumi::Shader>(std::string(MyShaderDir) + "painter_vertex.glsl", std::string(MyShaderDir) + "field_visualizer.glsl");
		_framebuffer = std::make_shared<Kasumi::Framebuffer>(_width, _height, 0.4, 0.2, 1.0, 1.0);

		std::array<float, 24> screen = {
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

		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &screen[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}
	void update(double dt) final
	{
		_shader->use();
		mVector2 screen(_width, _height);
		_shader->uniform("iResolution", screen);
		glBindVertexArray(_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		_framebuffer->use();
		_shader->use();
		_shader->uniform("iResolution", screen);
		glBindVertexArray(_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		_framebuffer->unuse();
		_framebuffer->render();
	}

private:
	Kasumi::ShaderPtr _shader;
	Kasumi::FramebufferPtr _framebuffer;

	unsigned int _vao;
};

auto main() -> int
{
	std::make_shared<Painter>()->launch();
	return 0;
}