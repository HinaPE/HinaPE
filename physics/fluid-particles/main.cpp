#include "renderer3D/renderer3D.h"
#include "sph_solver.h"
#include "pcisph_solver.h"
#include "pbf_solver.h"

#include "export_to_xyz.h"

using SolverType =
//		HinaPE::SPHSolver;
//		HinaPE::PCISPHSolver;
		HinaPE::PBFSolver;
using SolverDataType = SolverType::Data;

struct NeighborSearchVisualization : public Kasumi::ObjectParticles3D
{
	explicit NeighborSearchVisualization(std::shared_ptr<SolverDataType> data) : _data(std::move(data))
	{
		NAME = "NSV";
		track(&_neighbors);
	}
	std::shared_ptr<SolverDataType> _data;
	std::vector<mVector3> _neighbors;

	void on()
	{
		if (_data->_inst_id < 0 || _data->_inst_id >= _data->_positions.size())
			return;

		auto origin = _data->_positions[_data->_inst_id];
		for (auto &neighbor: _data->_neighbor_lists[_data->_inst_id])
			_neighbors.push_back(_data->_positions[neighbor]);
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
};

auto main() -> int
{
	// prepare solver
	auto solver = std::make_shared<SolverType>();
	auto data = std::make_shared<SolverDataType>();

	auto domain = std::make_shared<HinaPE::BoxDomain>();
	auto emitter = std::make_shared<HinaPE::VolumeParticleEmitter3>();
	solver->_data = data;
	solver->_domain = domain;
	solver->_emitter = emitter;

	auto vis = std::make_shared<NeighborSearchVisualization>(data);

	// setup renderer
	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(domain);
		scene->add(data);
		scene->add(vis);
		scene->_scene_opt._particle_mode = true;
		solver->init();
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._debugger = [&]()
	{
		vis->render();
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._key = [&](int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_H && action == GLFW_PRESS)
			vis->on();
		if (key == GLFW_KEY_H && action == GLFW_RELEASE)
			vis->off();
		if (key == GLFW_KEY_O && action == GLFW_PRESS)
			save_particles_as_pos(data->_positions.data()->data(), data->_positions.size(), std::string(DEFAULT_OUTPUT_DIR) + "output.xyz");
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(solver.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode(); // we use dark mode app to protect our eyes~ XD
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}