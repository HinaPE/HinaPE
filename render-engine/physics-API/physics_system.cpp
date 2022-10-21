#include "physics_system.h"

#include <utility>
#include <thread>

auto HinaPE::PhysicsSystem::_start_() -> void
{
    running = true;
}

auto HinaPE::PhysicsSystem::_pause_() -> void
{
    running = false;
}

auto HinaPE::PhysicsSystem::_tick_(float dt) -> void
{
//    std::visit([&](auto &k)
//               {
//                   k.simulate(dt);
//               }, kernel);
}

auto HinaPE::PhysicsSystem::instance() -> HinaPE::PhysicsSystem &
{
    static PhysicsSystem instance;
    return instance;
}

auto HinaPE::PhysicsSystem::destroy() -> void
{
    instance().~PhysicsSystem();
}

auto HinaPE::PhysicsSystem::_register_(unsigned int ID, std::shared_ptr<PhysicsObject> ptr) -> void // pass smart ptr by value
{
    physics_objects[ID] = std::move(ptr);
}
