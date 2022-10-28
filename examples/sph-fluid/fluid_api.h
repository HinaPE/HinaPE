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
#include "render-engine/scene/particles.h"
#include "render-engine/physics-API/particle_system_api.h"

class FluidAPI : public HinaPE::ParticleSystemAPI
{
public:
    void step(Scene_Particles *, float dt) final;

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
    FluidOpt fluid_opt;
    bool solver_prepared = false;
    std::shared_ptr<HinaPE::FluidEngine::ParticleSystemSolver3> solver_ptr;
    std::vector<std::shared_ptr<HinaPE::FluidEngine::ParticleEmitter3>> emitter_ptr_list;
    std::vector<std::shared_ptr<HinaPE::Collider3>> collider_ptr_list;
    HinaPE::BoundingBox3D particles_domain;
};

#endif //HINAPE_FLUID_API_H
