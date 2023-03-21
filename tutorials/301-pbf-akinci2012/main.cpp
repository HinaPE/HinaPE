#include "renderer3D/renderer3D.h"
#include "fluid-particles/pbf_solver_new.h"

using SolverType = HinaPE::PBFSolverNew;
using SolverDataType = SolverType::Data;

struct NeighborViewer : public Kasumi::ObjectParticles3D
{
	explicit NeighborViewer(std::shared_ptr<SolverDataType> data) : _data(std::move(data))
	{
		NAME = "NeighborViewer";
		track(&_neighbors);
	}
	void on()
	{
		if (_data->_inst_id < 0 || _data->_inst_id >= _data->Fluid.positions.size())
			return;

		auto origin = _data->Fluid.positions[_data->_inst_id];
		for (auto &neighbor: _data->NeighborList[_data->_inst_id])
			if (neighbor < _data->fluid_size())
				_neighbors.push_back(_data->Fluid.positions[neighbor]);
			else
				_neighbors.push_back(_data->Boundary.positions[neighbor - _data->fluid_size()]);
		_neighbors.push_back(origin);

		_shader->uniform("highlight_mode", true);
		_data->hide(true);
	}
	void off()
	{
		_neighbors.clear();
		_shader->uniform("highlight_mode", false);
		_data->hide(false);
	}
	std::shared_ptr<SolverDataType> _data;
	std::vector<mVector3> _neighbors;
};

class BoundaryViewer : public Kasumi::ObjectParticles3D
{
public:
	explicit BoundaryViewer(std::shared_ptr<SolverDataType> data) : _data(std::move(data))
	{
		NAME = "BoundaryViewer";
		track(&_data->Boundary.positions);
		_colors.resize(_data->boundary_size(), HinaPE::Color::BLUE);
		_color_map = &_colors;
		hide(true);
	}
	std::shared_ptr<SolverDataType> _data;
	std::vector<mVector3> _colors;
};

auto main() -> int
{
	auto solver = std::make_shared<SolverType>();
	solver->init();
	auto bv = std::make_shared<BoundaryViewer>(solver->_data);
	auto nv = std::make_shared<NeighborViewer>(solver->_data);

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(solver->_data);
		scene->add(solver->_domain);
		scene->add(bv);
		scene->add(nv);
		scene->_scene_opt._particle_mode = true;
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._key = [&](int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_Z && action == GLFW_PRESS)
			bv->hide(false);
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
			bv->hide(true);
		if (key == GLFW_KEY_H && action == GLFW_PRESS)
			nv->on();
		if (key == GLFW_KEY_H && action == GLFW_RELEASE)
			nv->off();
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(solver.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode(); // we use dark mode app to protect our eyes~ XD
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}