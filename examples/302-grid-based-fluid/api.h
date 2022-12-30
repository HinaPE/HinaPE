#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/app.h"
#include "renderer/scene/scene.h"
#include "common/logging.h"
#include "common/geometry/bounding_box3.h"
#include "common/geometry/plane3.h"
#include "common/geometry/box3.h"
#include "common/geometry/sphere3.h"
#include "common/geometry/implicit_surface_set3.h"
#include "common/geometry/rigid_body_collider3.h"
#include "fluid/emitter/volume_grid_emitter3.h"
#include "fluid/solver/grid/grid_fluid_solver3.h"
#include "fluid/solver/grid/grid_system_data3.h"

class GridBasedFluidExample:public Kasumi::Api
{
public:
    void step(float dt) final;
    void ui_sidebar() final;
    void sync();
private:
    struct GridBasedFluidOpt
    {
        float bounding_box_size = 1.f;
        // Grid Field
        float viscosity = 0.f;
        float density = 1000.f;
        float grid_resolution = 32.f;
        float grid_origin = 0.f;
        float grid_spacing = 0.05f;
        float grid_length = 1.f;
        float grid_width = 1.f;
        float grid_height = 1.f;
        float grid_initial_velocity = 0.f;
    } _opt;
private:
    auto phase1() -> bool;
    auto phase2() -> bool;
private:
    HinaPE::Fluid::GridFluidSolver3Ptr _fluid_solver;
    HinaPE::Fluid::VolumeGridEmitter3Ptr _fluid_emitter;
    HinaPE::RigidBodyCollider3Ptr _fluid_collider;

    Kasumi::ModelPtr _bounding_model;
    Kasumi::ModelPtr _grid_model;
    Kasumi::SceneObjectPtr _bounding_model_scene;
    Kasumi::SceneObjectPtr _grid_model_scene;

    bool _inited = false;

};
#endif //HINAPE_API_H
