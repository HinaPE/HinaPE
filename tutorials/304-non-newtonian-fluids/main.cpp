#include "renderer3D/renderer3D.h"
#include "fluid-particles/non_newtonian_pcisph_solver.h"
#include "fluid-particles/pcisph_temp.h"
#include "fluid-particles/pcisph_akinci.h"
#include "fluid-particles/pcisph_akinci_twoway.h"
#include "rigid/solver.h"
#include "export_to_xyz.h"

//using SolverType = HinaPE::PCISPHSolverNonNewtonian;
//using SolverType = HinaPE::PCISPHSolverTEMP;
//using SolverType = HinaPE::PCISPHAkinci;

using SolverType = HinaPE::PCISPHAkinciTwoWay;
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
            if (neighbor < _data->fluid_size())
                _neighbors.push_back(_data->Fluid.positions[neighbor]);
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
    void on()
    {
        hide(false);
    }
    void off()
    {
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
    auto nv = std::make_shared<NeighborViewer>(solver);

    // rigid solver
    auto solver_rigid = std::make_shared<HinaPE::RigidSolver>();
    //solver_rigid->add(solver->_cube);
    reactphysics3d::RigidBody* MyCube = solver_rigid->add(solver->_cube);

    auto domain_extent = solver->_domain->_extent;
    auto thickness = 0.1;

    auto top = std::make_shared<Kasumi::CubeObject>();
    top->POSE.position.y() = domain_extent.y() + thickness;
    top->POSE.scale = {domain_extent.x(), thickness, domain_extent.z()};
    top->_update_surface();

    auto bottom = std::make_shared<Kasumi::CubeObject>();
    bottom->POSE.position.y() = -domain_extent.y() - thickness;
    bottom->POSE.scale = {domain_extent.x(), thickness, domain_extent.z()};
    bottom->_update_surface();

    auto left = std::make_shared<Kasumi::CubeObject>();
    left->POSE.position.x() = -domain_extent.x() - thickness;
    left->POSE.scale = {thickness, domain_extent.y(), domain_extent.z()};
    left->_update_surface();

    auto right = std::make_shared<Kasumi::CubeObject>();
    right->POSE.position.x() = domain_extent.x() + thickness;
    right->POSE.scale = {thickness, domain_extent.y(), domain_extent.z()};
    right->_update_surface();

    auto front = std::make_shared<Kasumi::CubeObject>();
    front->POSE.position.z() = domain_extent.z() + thickness;
    front->POSE.scale = {domain_extent.x(), domain_extent.y(), thickness};
    front->_update_surface();

    auto back = std::make_shared<Kasumi::CubeObject>();
    back->POSE.position.z() = -domain_extent.z() - thickness;
    back->POSE.scale = {domain_extent.x(), domain_extent.y(), thickness};
    back->_update_surface();

    solver_rigid->add(top, HinaPE::RigidType::Static);
    solver_rigid->add(bottom, HinaPE::RigidType::Static);
    solver_rigid->add(left, HinaPE::RigidType::Static);
    solver_rigid->add(right, HinaPE::RigidType::Static);
    solver_rigid->add(front, HinaPE::RigidType::Static);
    solver_rigid->add(back, HinaPE::RigidType::Static);

    Kasumi::Renderer3D::DEFAULT_RENDERER._init = [&](const Kasumi::Scene3DPtr &scene)
    {
        scene->add(solver->_data);
        scene->add(solver->_domain);
        //scene->add(solver->_sphere);
        scene->add(solver->_cube);
        scene->add(bv);
        scene->add(nv);
        scene->_scene_opt._particle_mode = true;
    };

    Kasumi::Renderer3D::DEFAULT_RENDERER._step = [&](real dt)
    {
        solver->update(dt);
        auto force = solver->_data->ForceAndTorque.force[1];
        auto torque = solver->_data->ForceAndTorque.torque[1];
        solver_rigid->apply_force_and_torque(MyCube, force, torque);
        solver_rigid->update(solver->_opt.current_dt);
        solver_rigid->clear_force_and_torque(MyCube);
    };

    Kasumi::Renderer3D::DEFAULT_RENDERER._key = [&](int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
            solver->reset();
        if (key == GLFW_KEY_Z && action == GLFW_PRESS)
            bv->on();
        if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
            bv->off();
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