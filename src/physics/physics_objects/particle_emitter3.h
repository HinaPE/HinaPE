#ifndef HINAPE_PARTICLE_EMITTER3_H
#define HINAPE_PARTICLE_EMITTER3_H

#include "../common.h"

#include "particle_system_data3.h"

#include <random>
#include <variant>

namespace HinaPE
{
struct PointGenerator
{
    std::mt19937 rng;
};

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
    std::variant<PointGenerator> particle_generator;
};

}

#endif //HINAPE_PARTICLE_EMITTER3_H
