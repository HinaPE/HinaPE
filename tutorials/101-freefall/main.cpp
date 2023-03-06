#include "renderer3D/renderer3D.h"

class FreeFall : public Kasumi::Renderer3D
{
protected:
	void init() final
	{
		sphere = std::make_shared<Kasumi::SphereObject>();
		sphere->POSE.position = {0, 5, 0};
		add_obj(sphere);

		cube = std::make_shared<Kasumi::CubeObject>();
		cube->POSE.position = {0, 0, 0};
		add_obj(cube);
	}
	void step(real dt) final
	{
		HinaPE::TimeIntegration::semi_implicit_euler(dt, 1, sphere->POSE.position, sphere->POSE.position, {0, -9.8, 0});
	}

	Kasumi::SphereObjectPtr sphere;
	Kasumi::CubeObjectPtr cube;
};

auto main() -> int
{
	std::make_shared<FreeFall>()->launch();
	return 0;
}