#ifndef HINAPE_PARTICLE_SYSTEM_API_H
#define HINAPE_PARTICLE_SYSTEM_API_H

#include <memory>

class Scene_Particles;

namespace HinaPE
{
class ParticleSystemAPI
{
public:
    virtual void step(Scene_Particles *_scene_particles, float dt) = 0;
};
}

#endif //HINAPE_PARTICLE_SYSTEM_API_H
