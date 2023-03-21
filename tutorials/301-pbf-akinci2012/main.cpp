#include "renderer3D/renderer3D.h"
#include "fluid-particles/pbf_solver_new.h"

using SolverType = HinaPE::PBFSolverNew;
using SolverDataType = SolverType::Data;

class BoundaryViewer : public Kasumi::ObjectParticles3D
{
public:
	explicit BoundaryViewer(std::shared_ptr<SolverDataType> data) : _data(std::move(data))
	{
		NAME = "BoundaryViewer";
		track(&_data->Boundary.positions);
		_colors.resize(_data->Boundary.positions.size(), HinaPE::Color::BLUE);
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

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(solver->_data);
		scene->add(solver->_domain);
		scene->add(bv);
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
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(solver.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode(); // we use dark mode app to protect our eyes~ XD
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}