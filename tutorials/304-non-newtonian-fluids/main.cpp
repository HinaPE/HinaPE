#include "renderer3D/renderer3D.h"
#include "fluid-particles/non_newtonian_pcisph_solver.h"
#include "export_to_xyz.h"

using SolverType = HinaPE::PCISPHSolverNonNewtonian;
using SolverDataType = SolverType::Data;

#ifdef WIN32
#define ParticleToObj "particles2obj.exe"
#else
#define ParticleToObj "particles2obj"
#endif

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

        _neighbors.clear();

        auto origin = _data->Fluid.positions[_data->_inst_id];
        for (auto &neighbor: _data->FluidNeighborList[_data->_inst_id])
            _neighbors.push_back(_data->Fluid.positions[neighbor]);
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
};

auto main() -> int
{
    auto solver = std::make_shared<SolverType>();
    solver->init();
    auto nv = std::make_shared<NeighborViewer>(solver);

    Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
    {
        scene->add(solver->_data);
        scene->add(solver->_domain);
        scene->add(nv);
        scene->_scene_opt._particle_mode = true;
    };

    Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
    {
        solver->update(dt);
    };

    Kasumi::Renderer3D::DEFAULT_RENDERER._key = [&](int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
            solver->reset();
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