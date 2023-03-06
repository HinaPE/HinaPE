#include "renderer3D/renderer3D.h"

class SimpleFreeFallSystem
{
public:
	void add_sphere() {}
	void add_box() {}
	void add_plane() {}
	void update(real dt) {}
};

//class FreeFall : public Kasumi::Renderer3D
//{
//protected:
//	void init() final
//	{
//		sphere = std::make_shared<Kasumi::SphereObject>();
//		sphere->POSE.position = {0, 5, 0};
//		add_obj(sphere);
//
//		cube = std::make_shared<Kasumi::CubeObject>();
//		cube->POSE.position = {0, 0, 0};
//		add_obj(cube);
//	}
//	void step(real dt) final
//	{
//		auto a = mVector3::Zero();
//		HinaPE::TimeIntegration::semi_implicit_euler(dt, 1, sphere->POSE.position, a, {0, -9.8, 0});
//	}
//
//	Kasumi::SphereObjectPtr sphere;
//	Kasumi::CubeObjectPtr cube;
//};

auto main() -> int
{
	std::make_shared<SimpleFreeFallSystem>();
//	std::make_shared<FreeFall>()->launch();
	return 0;
}