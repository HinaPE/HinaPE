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
#include "common/grid/cell_centered_scalar_grid3.h"
#include "fluid/emitter/volume_grid_emitter3.h"
#include "fluid/solver/grid/smoke/grid_smoke_solver3.h"
#include "fluid/solver/grid/grid_system_data3.h"
#include "fluid/solver/grid/cubic_semi_lagrangian3.h"

class GridBasedFluidExample:public Kasumi::Api
{
public:
    void prepare() override;
    void step(float dt) final;
    void ui_sidebar() final;
    void sync();
private:
    struct GridBasedFluidOpt
    {
        float bounding_box_size = 1.f;

        // Grid Field
        mVector3 grid_resolution = mVector3(64, 64, 64);
        mVector3 grid_origin = mVector3(0.f, 0.f, 0.f);
        mVector3 grid_spacing = mVector3(0.05f, 0.05f, 0.05f);



    } _opt;
private:
    auto phase1() -> bool;
    auto phase2() -> bool;
private:
    // physics part
    HinaPE::Fluid::GridFluidSolver3Ptr _fluid_solver;
    HinaPE::Fluid::VolumeGridEmitter3Ptr _fluid_emitter;
    HinaPE::RigidBodyCollider3Ptr _fluid_collider;

    // scene part
    Kasumi::SceneObjectPtr _fluid_obj;
    Kasumi::SceneObjectPtr _bbox_obj;

};
#endif //HINAPE_API_H
