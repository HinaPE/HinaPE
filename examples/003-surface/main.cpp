#include "renderer/app.h"
#include "_geometry/sphere3.h"

#include <iostream>

class Surface : public Kasumi::Api
{
public:
	void add_sphere(const mVector3 &center, real radius)
	{
		auto sphere = std::make_shared<HinaPE::Sphere3>(center, radius);
		auto id = _scene->add_object(std::make_shared<Kasumi::Model>("sphere", Color::RED));
		_sphere_obj = _scene->get_object(id);
		_sphere_obj->position() = center;
		_sphere_obj->scale() = mVector3(radius, radius, radius);
	}
	void prepare() final
	{
		mVector3 center = mVector3(0, 1, 0);
		real radius = 1;
		add_sphere(center, radius);

		_lines = std::make_shared<Kasumi::Lines>();

		auto camera = _scene->get_camera();
		const auto &eye_pose = camera->_opt.position;
		_lines->add(eye_pose, center);
	}
	void step(float dt) final {}
	void ui_sidebar() final {}
	void mouse_cursor(double x_pos, double y_pos) final
	{
		std::cout << x_pos << ", " << y_pos << std::endl;
	}

private:
	Kasumi::SceneObjectPtr _sphere_obj;

	Kasumi::LinesPtr _lines;
};

auto main() -> int
{
	std::make_shared<Kasumi::Renderer>("empty.txt")
			->load_api(std::make_shared<Surface>())
			->launch();
	return 0;
}