#include "renderer3D/renderer3D.h"

class Data : public Kasumi::ObjectGrid3D
{
public:
	Data()
	{
		bunny = std::make_shared<Kasumi::BunnyObject>();
		_data = bunny->voxelize();
		track(&_data);
	}

	HinaPE::Geom::DataGrid3<int> _data;
	Kasumi::BunnyObjectPtr bunny;
};

class BoxDomain : public Kasumi::CubeObject
{
public:
	BoxDomain()
	{
		NAME = "Domain";
		_switch_surface();
		_switch_bbox();
		flip_normal(); // for inner collision
		set_color(HinaPE::Color::WHITE);
	}
};

struct Fluid : public Kasumi::ObjectParticles3D
{
	Fluid()
	{
		positions = std::make_shared<Kasumi::BunnyObject>()->generate_surface();
		track(&positions);
	}
	std::vector<mVector3> positions;
};

auto main() -> int
{
	auto domain = std::make_shared<BoxDomain>();
	auto fluid = std::make_shared<Fluid>();
	auto data = std::make_shared<Data>();

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(domain);
		scene->add(fluid);
//		scene->add(data);
		scene->_scene_opt._particle_mode = true;
	};
	Kasumi::Renderer3D::DEFAULT_RENDERER.close_benchmark();
	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode();
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}
