#ifndef HINAPE_SPH_KERNEL_H
#define HINAPE_SPH_KERNEL_H

#include "../../common.h"

#include "../../util/lib/frame.h"
#include "../../physics_objects/particle_system_data3.h"

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

        Vector3D gravity = Vector3D(0.0, -9.8, 0.0);
    };
    Opt opt;

private:

private:
    ParticleSystemData3Ptr data_ptr;
    ParticleSystemData3::VectorData new_positions;
    ParticleSystemData3::VectorData new_velocities;
    // TODO: add colliders
    // TODO: add emitters
    // TODO: add environments (winds etc.)
};
}

#endif //HINAPE_SPH_KERNEL_H
