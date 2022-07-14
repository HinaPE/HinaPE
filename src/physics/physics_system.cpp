#include "physics_system.h"

void HinaPE::PhysicsSystem::tick(float dt)
{
    std::visit([&](auto &k)
               {
                   k.simulate(dt);
               }, kernel);
}
