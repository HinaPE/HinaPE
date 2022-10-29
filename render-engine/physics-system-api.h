#ifndef HINAPE_PHYSICS_SYSTEM_API_H
#define HINAPE_PHYSICS_SYSTEM_API_H

#include <memory>
#include <optional>
#include <functional>

namespace Gui
{
class Manager;
class Widgets;
}

class Scene;
class Undo;
class Scene_Item;
class Scene_Object;
class Scene_Particles;

using Scene_Maybe = std::optional<std::reference_wrapper<Scene_Item>>;

namespace HinaPE
{
class PhysicsSystemAPI
{
public:
    virtual void step(Scene_Particles *_scene_particles, float dt) = 0;
    virtual void gui(Gui::Manager &manager, Scene &scene, Undo &undo, Gui::Widgets &widgets, Scene_Maybe obj, int &index) = 0;
};
class ParticleSystemAPI : public virtual PhysicsSystemAPI
{
protected:
    std::shared_ptr<Scene_Particles> _sp;
};
class ClothSystemAPI : public virtual PhysicsSystemAPI
{
protected:
    std::shared_ptr<Scene_Object> _so;
};
class RigidBodySystemAPI : public virtual PhysicsSystemAPI
{
protected:
    std::shared_ptr<Scene_Object> _so;
};
}

#endif //HINAPE_PHYSICS_SYSTEM_API_H
