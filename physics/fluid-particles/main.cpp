#include "renderer3D/renderer3D.h"
#include "sph_solver.h"

struct NeighborSearchVisualization : public Kasumi::ObjectLines3DInstanced
{
	explicit NeighborSearchVisualization(const HinaPE::SPHSolverDataPtr &data) : _data(data) {}
	HinaPE::SPHSolverDataPtr _data;

	void load(size_t i)
	{
		auto origin = _data->_positions[i];
		load(origin);
	}

	void load(const mVector3 &origin)
	{
		clear();
		_data->_neighbor_search->for_each_nearby_point(origin, 3, [&](size_t, const mVector3 &p)
		{
			add(origin, p);
		});
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


	// set up init & step
	renderer->_init = [&](const Kasumi::Scene3DPtr &scene)
	{
//		scene->add(data);
		scene->add(domain);
		scene->add(emitter);
		scene->add(vis);
	};

	renderer->_step = [&](real dt)
	{
		solver->update(dt);
		vis->load(mVector3(0, 0, 0));
	};


	renderer->inspect(solver.get());

	// launch renderer
	renderer->launch();
	return 0;
}