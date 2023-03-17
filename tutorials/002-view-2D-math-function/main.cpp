#include "renderer2D/renderer2D.h"

auto function(real x) -> real
{
	return 1000 * x * x;
}

auto main() -> int
{
	auto renderer = std::make_shared<Kasumi::Renderer2D>();

	renderer->_init = [&](const Kasumi::Scene2DPtr &scene)
	{
		auto top = std::make_shared<Kasumi::Rectangle2DObject>();
		top->resize(1000, 2);
		top->POSE.position = mVector2(0, 350);
		scene->add(top);

		auto bottom = std::make_shared<Kasumi::Rectangle2DObject>();
		bottom->resize(1000, 2);
		bottom->POSE.position = mVector2(0, -350);
		scene->add(bottom);

		auto left = std::make_shared<Kasumi::Rectangle2DObject>();
		left->resize(2, 700);
		left->POSE.position = mVector2(-500, 0);
		scene->add(left);

		auto right = std::make_shared<Kasumi::Rectangle2DObject>();
		right->resize(2, 700);
		right->POSE.position = mVector2(500, 0);
		scene->add(right);

		auto x_axis = std::make_shared<Kasumi::Rectangle2DObject>();
		x_axis->resize(1000, 2);
		x_axis->set_color(HinaPE::Color::ORANGE);
		scene->add(x_axis);

		auto y_axis = std::make_shared<Kasumi::Rectangle2DObject>();
		y_axis->resize(2, 700);
		y_axis->set_color(HinaPE::Color::BLUE);
		scene->add(y_axis);

		auto curve = std::make_shared<Kasumi::Curve2DObject>();
		curve->set_function(function);
		curve->set_color(HinaPE::Color::RED);
		scene->add(curve);
	};
	renderer->launch();
	return 0;
}