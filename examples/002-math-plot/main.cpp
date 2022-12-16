#include "glad/glad.h"

#include "backends/platform.h"
#include "backends/shader.h"
#include "backends/framebuffer.h"
#include "backends/model.h"

#include "implot.h"

#include <list>
#include <thread>

class MathPlot final : public Kasumi::App
{
public:
	MathPlot() : App(1500, 600, "Math Function Plotter")
	{
		_math_func = [](float x) -> float
		{
			return std::sin(x) * std::cos(8 * x);
		};
	}
	void prepare() final
	{
		_shader = std::make_shared<Kasumi::Shader>(std::string(MyShaderDir) + "math_plot_vertex.glsl", std::string(MyShaderDir) + "math_plot_fragment.glsl");
		_lines = std::make_shared<Kasumi::Lines>();
	}
	void draw()
	{
		_lines->clear();
		_lines->add({-1, 0, 0}, {1, 0, 0}, Color::RED); // x axis
		_lines->add({0, -1, 0}, {0, 1, 0}, Color::GREEN); // y axis

		float x = start_x + step * static_cast<float>(current_sample++);
		float y = _math_func(x);
		res.emplace_back(x, y);
		if (res.size() > samples)
			res.pop_front();

		float x_middle = res.front().first + step * static_cast<float>(samples) / 2;

		float y_max = std::max_element(res.begin(), res.end(), [](const auto &a, const auto &b) { return a.second < b.second; })->second;
		float y_min = std::min_element(res.begin(), res.end(), [](const auto &a, const auto &b) { return a.second < b.second; })->second;
		float y_middle = (y_max + y_min) / 2;
		float y_size = y_max - y_min;
		auto iter = res.begin();
		for (int i = 0; i < res.size() - 1; ++i)
		{
			float x0 = (iter->first - x_middle) / x_size * 2;
			float x1 = (iter->first - x_middle) / x_size * 2;
			float y0 = (iter->second - y_middle) / y_size * 1.8f;
			++iter;
			float y1 = (iter->second - y_middle) / y_size * 1.8f;
			_lines->add({x0, y0, 0}, {x1, y1, 0}, Color::BLUE);
		}
		_lines->render(_shader);
	}
	void update(double dt) final { draw(); }

private:
	Kasumi::ShaderPtr _shader;
	Kasumi::LinesPtr _lines; // math function lines
	std::function<float(float)> _math_func;

private:
	int current_sample = 0;
	const float start_x = 0;
	const float step = 0.01;
	const int samples = 1000;
	const float x_size = step * static_cast<float>(samples); // map to [-1, 1]
	std::list<std::pair<float, float>> res;
};

auto main() -> int
{
	auto plot = std::make_shared<MathPlot>();
	plot->launch();
	return 0;
}