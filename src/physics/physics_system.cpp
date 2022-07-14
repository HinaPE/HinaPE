#include "physics_system.h"

#include <utility>

void HinaPE::PhysicsSystem::tick(float dt)
{
    std::visit([&](auto &k)
               {
                   k.simulate(*this, dt);
               }, kernel);
}


HinaPE::PhysicsSystem &HinaPE::PhysicsSystem::instance()
{
    static PhysicsSystem instance;
    return instance;
}

void HinaPE::PhysicsSystem::destroy()
{
    instance().~PhysicsSystem();
}

void HinaPE::PhysicsSystem::_register_(unsigned int ID, std::shared_ptr<PhysicsObject> ptr) // pass smart ptr by value
{
    physics_objects[ID] = std::move(ptr);
}

const std::map<unsigned int, std::shared_ptr<HinaPE::PhysicsObject>> &HinaPE::PhysicsSystem::objects() const
{
    return physics_objects;
}
