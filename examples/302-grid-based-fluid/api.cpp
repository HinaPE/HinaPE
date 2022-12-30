#include "api.h"
#include "imgui.h"

void GridBasedFluidExample::step(float dt)
{
    if(_fluid_solver == nullptr)
        return;

    static HinaPE::Frame frame(0, 1.0/60.0);
    _fluid_solver->update(frame);

    sync();
}

void GridBasedFluidExample::ui_sidebar()
{
    if(!_inited)
    {
        HinaPE::Logging::mute();
        _bounding_model = std::make_shared<Kasumi::Model>("cube", Color::NO_COLORS);
        _bounding_model_scene = _scene->get_object(_scene->add_object(_bounding_model));
        _bounding_model->_opt.render_surface = false; // dont render the surface
        _bounding_model->_opt.render_bbox = true; // render the bounding box instead
        _bounding_model->_opt.bbox_color = Color::GRAY;
        _inited = true;
    }
    static int phase = 0;
    switch(phase)
    {
        case 0:
            phase += phase1();
            break;
        case 1:
            phase += phase2();
            break;
        default:
            break;
    }
}

void GridBasedFluidExample::sync()
{

}

auto GridBasedFluidExample::phase1() -> bool
{
    if (ImGui::CollapsingHeader("Create Fluid Domain"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        ImGui::SliderFloat("Size", &_opt.bounding_box_size, 0.01f, 10.0f, "%.2f");
        _bounding_model_scene->scale() = mVector3(_opt.bounding_box_size, _opt.bounding_box_size, _opt.bounding_box_size);
        if (ImGui::Button("Add"))
        {
            _bounding_model->_opt.bbox_color = Color::BLACK;
            return true;
        }
    }
    return false;
}

auto GridBasedFluidExample::phase2() -> bool
{
    if (ImGui::CollapsingHeader("Create Fluid Solver"), ImGuiTreeNodeFlags_DefaultOpen) {
        ImGui::SliderFloat("Grid Spacing", &_opt.grid_spacing, 0.01f, 0.1f, "%.2f");
        ImGui::SliderFloat("Viscosity", &_opt.viscosity, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Density", &_opt.density, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Resolution", &_opt.grid_resolution, 0.0f, 100.0f, "%.2f");
        ImGui::SliderFloat("Grid Initial Velocity", &_opt.grid_initial_velocity, 0.0f, 1.0f, "%.2f");
        if(ImGui::Button("Add"))
        {
            auto plane = HinaPE::Plane3::builder()
                    .withNormal({0, 1, 0})
                    .withPoint({0, 0.25 * _opt.bounding_box_size, 0})
                    .makeShared();
            auto sphere = HinaPE::Sphere3::builder()
                    .withCenter({_bounding_model_scene->position().x, _bounding_model_scene->position().y, _bounding_model_scene->position().z})
                    .withRadius(0.15 * _opt.bounding_box_size)
                    .makeShared();
            auto surfaceSet = HinaPE::ImplicitSurfaceSet3::builder()
                    .withExplicitSurfaces({plane, sphere})
                    .makeShared();
            _fluid_emitter = HinaPE::Fluid::VolumeGridEmitter3::builder()
                    .withSourceRegion(surfaceSet)
                    .makeShared();
            auto box = HinaPE::Box3::builder()
                    .withIsNormalFlipped(true)
                    .withBoundingBox(HinaPE::BoundingBox3D({-_opt.bounding_box_size, -_opt.bounding_box_size, -_opt.bounding_box_size}, {_opt.bounding_box_size, _opt.bounding_box_size, _opt.bounding_box_size}))
                    .makeShared();
            _fluid_collider = HinaPE::RigidBodyCollider3::builder()
                    .withSurface(box)
                    .makeShared();
            _fluid_solver = HinaPE::Fluid::GridFluidSolver3::builder()
                    .withResolution({_opt.grid_resolution, _opt.grid_resolution * 2, _opt.grid_resolution})
                    .withDomainSizeX(1.0)
                    .makeShared();
            _fluid_solver->setEmitter(_fluid_emitter);
            _fluid_solver->setCollider(_fluid_collider);
            _fluid_solver->setViscosityCoefficient(_opt.viscosity);

            _grid_model = std::make_shared<Kasumi::Model>("cube", Color::BLUE);
            _grid_model_scene = _scene->get_object(_scene->add_object(_grid_model));
            _grid_model_scene->scale() = mVector3(_opt.grid_spacing, _opt.grid_spacing, _opt.grid_spacing);
            _grid_model->_opt.render_surface = false; // dont render the surface
            _grid_model->_opt.render_bbox = true; // render the bounding box instead
            _grid_model->_opt.bbox_color = Color::GRAY;

            sync();
            return true;
        }
    }
    return false;
}