#ifndef HINAPE_SPH_KERNEL_H
#define HINAPE_SPH_KERNEL_H

#include "../../common.h"

#include "../../util/lib/frame.h"
#include "../../physics_objects/particle_system_data3.h"
#include "../../physics_objects/particle_emitter3.h"
#include "../../physics_objects/particle_system.h"

namespace HinaPE
{
class PhysicsSystem;

class SPHKernel
{
public:
    auto simulate(float dt) -> void;

    struct Opt
    {
        Frame frame;
        bool is_fixed_time_step = true;
        unsigned int sub_time_step = 1;
        double current_time = 0.0;

    };
    Opt opt;
    float h, MASS, self_density, eosScale, eosExponent;
    float targetDensity;
    std::vector<Particle*> particles;
    Particle ** particleTable;

public:
    auto init_particle_system() -> void;
    auto buildTable() ->void;
    static auto getHash(Vec3 cell) -> unsigned int;
    auto getCell(Particle* p) const -> Vec3;
//    float cubic_kernel(float r_norm);
//    float cubic_kernel_derivative(float r_norm);


private:

private:
    ParticleSystemData3Ptr data_ptr;
    ParticleSystemData3::VectorData new_positions;
    ParticleSystemData3::VectorData new_velocities;
    // TODO: add colliders
    ParticleEmitter3Ptr emitter_ptr;
    // TODO: add environments (winds etc.)
};
}

#endif //HINAPE_SPH_KERNEL_H
