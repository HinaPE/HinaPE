#include "renderer3D/renderer3D.h"
#include "experimental/Voxelizer.h"
#include "experimental/Mesh.h"

class Data : public Kasumi::ObjectGrid3D
{
public:
	Data()
	{
		auto sphere = cs224::Mesh::createSphere(cs224::Vector3f(0, 0, 0), 1);
		cs224::Voxelizer::Result result;
		cs224::Voxelizer::voxelize(sphere, 0.02, result);

		auto grid = result.grid;
		auto bounds = result.bounds;
		auto cellSize = result.cellSize;
		auto spacing = {cellSize, cellSize, cellSize};
		HinaPE::Math::Size3 resolution = {grid.size().x(), grid.size().y(), grid.size().z()};
		_data.resize(resolution, spacing);

		for (int z = 0; z < resolution.z; ++z)
			for (int y = 0; y < resolution.y; ++y)
				for (int x = 0; x < resolution.x; ++x)
					_data(x, y, z) = grid.value(x, y, z);

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
