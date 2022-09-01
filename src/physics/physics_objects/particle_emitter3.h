#ifndef HINAPE_PARTICLE_EMITTER3_H
#define HINAPE_PARTICLE_EMITTER3_H

#include "particle_system_data3.h"

namespace HinaPE
{
class ParticleEmitter3
{
public:
    auto update() -> void;

public:
    ParticleEmitter3() = default;

    struct Opt
    {
        bool is_enabled = true;
    };
    Opt opt;

private:
    auto on_update() -> void;

private:
    ParticleSystemData3Ptr data_ptr;
};
}

#endif //HINAPE_PARTICLE_EMITTER3_H
