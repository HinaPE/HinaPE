#include "api.h"
#include "imgui.h"

void GridBasedFluidExample::prepare() {
    HinaPE::Logging::mute();
    auto bbox_model = std::make_shared<Kasumi::Model>("cube", Color::NO_COLORS);
    bbox_model->_opt.render_surface = false; // dont render the surface
    bbox_model->_opt.render_bbox = true; // render the bounding box instead
    bbox_model->_opt.bbox_color = Color::GRAY;
    _bbox_obj = _scene->get_object(_scene->add_object(bbox_model));
}

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
    std::vector<Kasumi::Pose> poses;
    size_t n = _fluid_solver->gridSystemData()->numberOfAdvectableScalarData();
    for(size_t i = 0; i < n; ++i){
        for(size_t j = 0; j < n; ++j){
            for(size_t k = 0; k < n; ++k){
                auto const &positions = _fluid_solver->gridOrigin() + _fluid_solver->gridSpacing() * HinaPE::Vector3D({i, j, k});
                Kasumi::Pose pose;
                pose.position = {positions.x, positions.y, positions.z};
                pose.scale = {0.01, 0.01, 0.01};
                poses.push_back(pose);
            }
        }
    }

    _fluid_obj->get_model()->clear_instances();
    if(!poses.empty())
        _fluid_obj->get_model()->add_instances(poses);
}

auto GridBasedFluidExample::phase1() -> bool
{
    if (ImGui::CollapsingHeader("Create Fluid Domain"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        ImGui::SliderFloat("Size", &_opt.bounding_box_size, 0.01f, 10.0f, "%.2f");
        _bbox_obj->scale() = mVector3(_opt.bounding_box_size, _opt.bounding_box_size, _opt.bounding_box_size);
        if (ImGui::Button("Add"))
        {
            _bbox_obj->get_model()->_opt.bbox_color = Color::BLACK;
            return true;
        }
    }
    return false;
}

auto GridBasedFluidExample::phase2() -> bool
{
    if (ImGui::CollapsingHeader("Create Fluid Solver"), ImGuiTreeNodeFlags_DefaultOpen) {
        ImGui::DragFloat3("resolution", &_opt.grid_resolution[0], 32.0f);
        ImGui::DragFloat3("grid_origin", &_opt.grid_origin[0], 0.0f);
        ImGui::DragFloat3("grid_spacing", &_opt.grid_spacing[0], 0.05f);

        if(ImGui::Button("Add"))
        {
            auto box = HinaPE::Box3::builder()
                    .withLowerCorner({0.45, -1, 0.45})
                    .withUpperCorner({0.55, 0.05, 0.55})
                    .makeShared();
            _fluid_emitter = HinaPE::Fluid::VolumeGridEmitter3::builder()
                    .withSourceRegion(box)
                    .withIsOneShot(false)
                    .makeShared();
            auto sphere = HinaPE::Sphere3::builder()
                    .withCenter({0.5, 0.3, 0.5})
                    .withRadius(0.075 * _opt.bounding_box_size)
                    .makeShared();
            _fluid_collider = HinaPE::RigidBodyCollider3::builder()
                    .withSurface(sphere)
                    .makeShared();
            _fluid_solver = HinaPE::Fluid::GridFluidSolver3::builder()
                            .withResolution({_opt.grid_resolution[0], 2 * _opt.grid_resolution[0], _opt.grid_resolution[0]})
                            .withDomainSizeX(1.0)
                            .makeShared();
            _fluid_solver->setAdvectionSolver(std::make_shared<HinaPE::CubicSemiLagrangian3>());
            auto grids = _fluid_solver->gridSystemData();
            HinaPE::BoundingBox3D domain = grids->boundingBox();
            _fluid_solver->setEmitter(_fluid_emitter);
            unsigned int _smokeDensityDataId = grids->addAdvectableScalarData(
                    std::make_shared<HinaPE::CellCenteredScalarGrid3::Builder>(), 0.0);
            unsigned int _temperatureDataId = grids->addAdvectableScalarData(
                    std::make_shared<HinaPE::CellCenteredScalarGrid3::Builder>(), 0.0);
            _fluid_emitter->addStepFunctionTarget(_fluid_solver->gridSystemData()->advectableScalarDataAt(_smokeDensityDataId), 0, 1);
            _fluid_emitter->addStepFunctionTarget(_fluid_solver->gridSystemData()->advectableScalarDataAt(_temperatureDataId), 0, 1);
            _fluid_solver->setCollider(_fluid_collider);

            auto fluid_model = std::make_shared<Kasumi::Model>("sphere", Color::BLUE);
            fluid_model->instancing();
            _fluid_obj = _scene->get_object(_scene->add_object(fluid_model));

            sync();
            return true;
        }
    }
    return false;
}


