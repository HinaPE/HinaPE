#ifndef HINAPE_CLOTH_SYSTEM_API_H
#define HINAPE_CLOTH_SYSTEM_API_H

class Scene_Object;

namespace HinaPE
{
class ClothSystemAPI
{
public:
    virtual void step(Scene_Object *_scene_particles, float dt) = 0;
};
}

#endif //HINAPE_CLOTH_SYSTEM_API_H
