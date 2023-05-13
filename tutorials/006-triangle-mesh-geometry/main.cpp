#include "renderer3D/renderer3D.h"
#include "experimental/Voxelizer.h"
#include "experimental/Mesh.h"

class Data : public Kasumi::ObjectGrid3D
{
public:
	Data()
	{
		auto bunny = std::make_shared<Kasumi::BunnyObject>();
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

auto main() -> int
{
	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		auto domain = std::make_shared<BoxDomain>();
		auto data = std::make_shared<Data>();
		scene->add(data);
		//		scene->add(data->bunny);
		scene->add(domain);
	};
	Kasumi::Renderer3D::DEFAULT_RENDERER.close_benchmark();
	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode();
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}
