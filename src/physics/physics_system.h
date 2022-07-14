#ifndef HINAPE_PHYSICS_SYSTEM_H
#define HINAPE_PHYSICS_SYSTEM_H

#include "pbd/pbd_kernel.h"
#include "xpbd/xpbd_kernel.h"
#include "physics_objects/physics_object.h"

namespace HinaPE
{

class PhysicsSystem
{
public:
    void tick(float dt);

public:
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;
    PhysicsSystem(const PhysicsSystem &) = delete;
    PhysicsSystem &operator=(const PhysicsSystem &) = delete;
    PhysicsSystem(PhysicsSystem &&) = delete;
    PhysicsSystem &operator=(PhysicsSystem &&) = delete;

private:
    std::variant<PBDKernel, XPBDKernel> kernel;
    std::map<int, PhysicsObject> physics_objects;
    std::map<int, PhysicsObject> erased_physics_objects;
};

}


#endif //HINAPE_PHYSICS_SYSTEM_H
