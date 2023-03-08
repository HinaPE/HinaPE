#include <utility>

#include "renderer3D/renderer3D.h"
#include "sph_solver.h"

struct NeighborSearchVisualization : public Kasumi::ObjectLines3DInstanced
{
	explicit NeighborSearchVisualization(HinaPE::SPHSolverDataPtr data) : _data(std::move(data)) {}
	HinaPE::SPHSolverDataPtr _data;

	void load(size_t i)
	{
		if (i < 0 || i >= _data->_positions.size())
			return;
		auto origin = _data->_positions[i];
		load(origin);
	}

	void load(const mVector3 &origin)
	{
		clear();
		_data->_neighbor_search->for_each_nearby_point(origin, 1, [&](size_t, const mVector3 &p)
		{
			add(origin, p);
		});
	}
};

struct DensityMonitor : public Kasumi::INSPECTOR
{
	explicit DensityMonitor(HinaPE::SPHSolverDataPtr data) : _data(std::move(data)) {}
	HinaPE::SPHSolverDataPtr _data;

protected:
	void INSPECT() final
	{
		if (_data->_inst_id >= 0 && _data->_inst_id < _data->_densities.size())
			ImGui::Text("Inst: %d, Density: %.3f", _data->_inst_id, _data->_densities[_data->_inst_id]);
	}
};

auto main() -> int
{
	// prepare solver
	auto renderer = std::make_shared<Kasumi::Renderer3D>();
	auto solver = std::make_shared<HinaPE::SPHSolver>();

	// solver init
	auto data = std::make_shared<HinaPE::SPHSolver::Data>();
	auto domain = std::make_shared<HinaPE::BoxDomain>();
	auto emitter = std::make_shared<HinaPE::PointParticleEmitter3>();
	solver->_data = data;
	solver->_domain = domain;
	solver->_emitter = emitter;

	auto vis = std::make_shared<NeighborSearchVisualization>(data);
	auto des_vis = std::make_shared<DensityMonitor>(data);


	// set up init & step & debugger
	renderer->_init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(data);
		scene->add(domain);
		scene->add(emitter);
		scene->add(vis);
	};

	renderer->_step = [&](real dt)
	{
		solver->update(dt);
	};

	renderer->_debugger = [&]()
	{
		vis->load(data->_inst_id);
	};


	renderer->inspect(solver.get());
	renderer->inspect(des_vis.get());

	// launch renderer
	renderer->launch();
	return 0;
}