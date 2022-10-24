#include "fluid_api.h"

void FluidAPI::step(Scene_Particles *_scene_particles, float dt)
{
    auto &_p = _scene_particles->particles;
    auto size = solver_ptr->particleSystemData()->numberOfParticles();
    _p.resize(size);

    static HinaPE::FluidEngine::Frame frame(0, 1.0 / 60.0);
    solver_ptr->update(frame++);

    auto const &data = std::static_pointer_cast<HinaPE::FluidEngine::SphSolver3>(solver_ptr)->sphSystemData();
    HinaPE::FluidEngine::parallelFor((size_t) 0, (size_t) size, [&](size_t i)
    {
        auto &p = data->positions()[i];
        _p[i].pos = Vec3((float) p[0], (float) p[1], (float) p[2]);
    });

    std::cout << "Particles Size: " << _p.size() << std::endl;
}

void FluidAPI::load_solver()
{
    if (!emitter_ptr_list.empty() && !collider_ptr_list.empty() && !particles_domain.isEmpty() && !solver_prepared)
    {
        solver_ptr = HinaPE::FluidEngine::SphSolver3::builder().withTargetDensity(fluid_opt.target_density).withTargetSpacing(fluid_opt.target_spacing).makeShared();
        solver_ptr->setEmitter(emitter_ptr_list.back()); // TODO: to support more emitter
        solver_ptr->setCollider(collider_ptr_list.back()); // TODO: to support more collider
        if (fluid_opt.type == SPH)
        {
            std::static_pointer_cast<HinaPE::FluidEngine::SphSolver3>(solver_ptr)->setPseudoViscosityCoefficient(fluid_opt.pseudo_viscosity_coefficient);
        }
        solver_prepared = true;
    }
}

void FluidAPI::add_emitter(const std::shared_ptr<HinaPE::FluidEngine::ParticleEmitter3> &_emitter_ptr) { emitter_ptr_list.emplace_back(_emitter_ptr); }
void FluidAPI::add_collider(const std::shared_ptr<HinaPE::FluidEngine::Collider3> &_collider_ptr) { collider_ptr_list.emplace_back(_collider_ptr); }
void FluidAPI::assign_particles_domain(const HinaPE::FluidEngine::BoundingBox3D &_domain) { particles_domain = _domain; }
