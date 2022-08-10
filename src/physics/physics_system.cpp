#include "physics_system.h"

#include <utility>
#include "lib/point.h"
#include "lib/point2.h"

HinaPE::Point<int, 1> a;
HinaPE::Point<int, 2> b;

void HinaPE::PhysicsSystem::_tick_(float dt)
{
    std::visit([&](auto &k)
               {
                   k.simulate(*this, dt);
               }, kernel);
}

auto HinaPE::PhysicsSystem::instance() -> HinaPE::PhysicsSystem &
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
