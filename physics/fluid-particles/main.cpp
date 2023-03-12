#include "renderer3D/renderer3D.h"
#include "sph_solver.h"

struct NeighborSearchVisualization : public Kasumi::ObjectLines3DInstanced
{
	explicit NeighborSearchVisualization(HinaPE::SPHSolverDataPtr data) : _data(std::move(data)) {}
	HinaPE::SPHSolverDataPtr _data;

	void on() const { _data->_shader->uniform("highlight_mode", true); }
	void off() const { _data->_shader->uniform("highlight_mode", false); }
	void load()
	{
		if (_data->_inst_id < 0 || _data->_inst_id >= _data->_positions.size())
			return;
		auto origin = _data->_positions[_data->_inst_id];
		clear();
		auto neighbors = _data->_neighbor_lists[_data->_inst_id];
		neighbors.push_back(_data->_inst_id);
		_data->highlight(neighbors);
	}
};

auto main() -> int
{
	// prepare solver
	auto solver = std::make_shared<HinaPE::SPHSolver>();

	auto data = std::make_shared<HinaPE::SPHSolver::Data>();
	auto domain = std::make_shared<HinaPE::BoxDomain>();
	auto emitter = std::make_shared<HinaPE::VolumeParticleEmitter3>();
	emitter->POSE.position = {-0.8, 0.8, 0};
	emitter->POSE.euler = {0, 0, -90};
	emitter->POSE.scale = {0.1, 0.2, 0.1};
	solver->_data = data;
	solver->_domain = domain;
	solver->_emitter = emitter;

	auto vis = std::make_shared<NeighborSearchVisualization>(data);

	// setup renderer
	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(data);
		scene->add(domain);
		scene->add(vis);
		scene->_scene_opt._particle_mode = true;
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._debugger = [&]()
	{
		vis->load();
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._key = [&](int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_H && action == GLFW_PRESS)
			vis->on();
		if (key == GLFW_KEY_H && action == GLFW_RELEASE)
			vis->off();
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(solver.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}