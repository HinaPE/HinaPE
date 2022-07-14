#ifndef HINAPE_PHYSICS_SYSTEM_H
#define HINAPE_PHYSICS_SYSTEM_H

#include "pbd/pbd_kernel.h"
#include "xpbd/xpbd_kernel.h"
#include "physics_object.h"

namespace HinaPE
{
class PhysicsSystem // Singleton Pattern
{
public:
    static PhysicsSystem &instance();
    static void destroy();

public:
    void tick(float dt);
    void _register_(unsigned int ID, std::shared_ptr<PhysicsObject> ptr);
    [[nodiscard]] const std::map<unsigned int, std::shared_ptr<PhysicsObject>>&objects() const;

public:
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;
    PhysicsSystem(const PhysicsSystem &) = delete;
    PhysicsSystem &operator=(const PhysicsSystem &) = delete;
    PhysicsSystem(PhysicsSystem &&) = delete;
    PhysicsSystem &operator=(PhysicsSystem &&) = delete;

private:
    std::variant<PBDKernel, XPBDKernel> kernel;
    std::map<unsigned int, std::shared_ptr<PhysicsObject>> physics_objects;
    std::map<unsigned int, std::shared_ptr<PhysicsObject>> erased_physics_objects;
};
}


#endif //HINAPE_PHYSICS_SYSTEM_H
