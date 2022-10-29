#ifndef HINAPE_FLUID_API_H
#define HINAPE_FLUID_API_H

#include "common/geometry/bounding_box3.h"
#include "common/geometry/plane3.h"
#include "common/geometry/box3.h"
#include "common/geometry/rigid_body_collider3.h"
#include "common/geometry/sphere3.h"
#include "common/geometry/implicit_surface_set3.h"
#include "common/logging.h"
#include "fluid-engine/solver/particle/sph/sph_solver3.h"
#include "fluid-engine/emitter/volume_particle_emitter3.h"
#include "render-engine/scene/scene.h"
#include "render-engine/gui/manager.h"
#include "render-engine/physics-system-api.h"

class FluidAPI : public HinaPE::ParticleSystemAPI, public std::enable_shared_from_this<FluidAPI>
{
public:
    void step(Scene_Particles *, float dt) final;
    void gui(Gui::Manager &manager, Scene &scene, Undo &undo, Gui::Widgets &widgets, Scene_Maybe obj, int &index) override;

public:
    void load_solver();
    void add_emitter(const std::shared_ptr<HinaPE::FluidEngine::ParticleEmitter3>&);
    void add_collider(const std::shared_ptr<HinaPE::Collider3>&);
    void assign_particles_domain(const HinaPE::BoundingBox3D &);
    enum FluidType
    {
        SPH
    };
    struct FluidOpt
    {
        float target_density = 1000.f;
        float target_spacing = 0.05f;

        FluidType type = SPH;
        // SPH Field
        float pseudo_viscosity_coefficient = 0.f;
    };
    FluidOpt _fluid_opt;
    bool _solver_prepared = false;
    std::shared_ptr<HinaPE::FluidEngine::ParticleSystemSolver3> _solver_ptr;
    std::vector<std::shared_ptr<HinaPE::FluidEngine::ParticleEmitter3>> _emitter_ptr_list;
    std::vector<std::shared_ptr<HinaPE::Collider3>> _collider_ptr_list;
    HinaPE::BoundingBox3D _particles_domain;
};

#endif //HINAPE_FLUID_API_H
