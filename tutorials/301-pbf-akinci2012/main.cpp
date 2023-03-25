#include "renderer3D/renderer3D.h"
#include "fluid-particles/pbf_solver_new.h"
#include "export_to_xyz.h"

using SolverType = HinaPE::PBFSolverNew;
using SolverDataType = SolverType::Data;

struct NeighborViewer : public Kasumi::ObjectParticles3D
{
	explicit NeighborViewer(std::shared_ptr<SolverType> solver) : _solver(std::move(solver)), _data(_solver->_data)
	{
		NAME = "NeighborViewer";
		track(&_neighbors);
	}
	void on()
	{
		if (_data->_inst_id < 0)
			return;

		if (_data->p_debug.empty())
			return;

		if (_data->_inst_id >= _data->p_debug[iter].size())
			return;

		_neighbors.clear();

		auto origin = _data->p_debug[iter][_data->_inst_id];
		for (auto &neighbor: _data->neighbor_list_debug[_data->_inst_id])
			if (neighbor < _data->fluid_size())
				_neighbors.push_back(_data->p_debug[iter][neighbor]);
			else
				_neighbors.push_back(_data->Boundary.positions[neighbor - _data->fluid_size()]);
		_neighbors.push_back(origin);

		_shader->uniform("highlight_mode", true);
		_data->hide(true);

		_on = true;
	}
	void off()
	{
		_neighbors.clear();
		_shader->uniform("highlight_mode", false);
		_data->hide(false);

		_on = false;
	}

	std::shared_ptr<SolverType> _solver;
	std::shared_ptr<SolverDataType> _data;
	std::vector<mVector3> _neighbors;
	bool _on = false;
	int iter = 0;

protected:
	void INSPECT() final
	{
		if (!_on)
			return;

		if (ImGui::SliderInt("iter", &iter, 0, static_cast<int>(_data->p_debug.size() - 1)))
			on();
	}
};

struct BoundaryViewer : public Kasumi::ObjectParticles3D
{
public:
	explicit BoundaryViewer(std::shared_ptr<SolverDataType> data) : _data(std::move(data))
	{
		NAME = "BoundaryViewer";
		track(&_data->Boundary.positions);
		track_colormap(&_colors);
		_colors.resize(_data->boundary_size(), HinaPE::Color::BLUE);
		hide(true);
	}
	std::shared_ptr<SolverDataType> _data;
	std::vector<mVector3> _colors;

protected:
	void INSPECT() final {}
};

#ifdef WIN32
#define ParticleToObj "particles2obj.exe"
#else
#define ParticleToObj "particles2obj"
#endif

auto main() -> int
{
	auto solver = std::make_shared<SolverType>();
	solver->init();
	auto bv = std::make_shared<BoundaryViewer>(solver->_data);
	auto nv = std::make_shared<NeighborViewer>(solver);

	Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
	{
		scene->add(solver->_data);
		scene->add(solver->_domain);
		scene->add(solver->_sphere);
		scene->add(solver->_cube);
		scene->add(bv);
		scene->add(nv);
		scene->_scene_opt._particle_mode = true;
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
	{
		solver->update(dt);

//		static int frame_num = 0;
//		if (frame_num < 60)
//		{
//			std::thread(
//					[&]()
//					{
//						save_particles_as_pos(solver->_data->Fluid.positions.data()->data(), solver->_data->Fluid.positions.size(), std::string(DEFAULT_OUTPUT_DIR) + "frame_" + std::to_string(frame_num) + ".xyz");
//						const std::string command = std::string(DEFAULT_OUTPUT_DIR) + std::string(ParticleToObj) + " " + std::string(DEFAULT_OUTPUT_DIR) + "frame_" + std::to_string(frame_num) + ".xyz" + " " + std::string(DEFAULT_OUTPUT_DIR) + "frame_" + std::to_string(frame_num) + ".obj";
//						exec(command.c_str());
//					}).detach();
//			++frame_num;
//		}
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER._key = [&](int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
			solver->reset();
		if (key == GLFW_KEY_Z && action == GLFW_PRESS)
			bv->hide(false);
		if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
			bv->hide(true);
		if (key == GLFW_KEY_H && action == GLFW_PRESS)
			nv->on();
		if (key == GLFW_KEY_H && action == GLFW_RELEASE)
			nv->off();
		if (key == GLFW_KEY_O && action == GLFW_PRESS)
		{
			std::thread(
					[&]()
					{
						save_particles_as_pos(solver->_data->Fluid.positions.data()->data(), solver->_data->Fluid.positions.size(), std::string(DEFAULT_OUTPUT_DIR) + "frame_" + "test" + ".xyz");
						const std::string command = std::string(DEFAULT_OUTPUT_DIR) + std::string(ParticleToObj) + " " + std::string(DEFAULT_OUTPUT_DIR) + "frame_" + "test" + ".xyz" + " " + std::string(DEFAULT_OUTPUT_DIR) + "frame_" + "test" + ".obj";
						exec(command.c_str());
					}).detach();
		}
	};

	Kasumi::Renderer3D::DEFAULT_RENDERER.inspect(solver.get());
	Kasumi::Renderer3D::DEFAULT_RENDERER.dark_mode(); // we use dark mode app to protect our eyes~ XD
	Kasumi::Renderer3D::DEFAULT_RENDERER.launch();
	return 0;
}