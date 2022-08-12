#ifndef HINAPE_PHYSICS_SYSTEM_H
#define HINAPE_PHYSICS_SYSTEM_H

#include "kernels/pbd/pbd_kernel.h"
#include "kernels/xpbd/xpbd_kernel.h"
#include "kernels/fast-mass-spring/fms_kernel.h"
#include "collision/simple_collision.h"
#include "physics_object.h"

#include <vector>
#include <variant>
#include <map>

namespace HinaPE
{
class PhysicsSystem
{
public: // Singleton Pattern
    static auto instance() -> PhysicsSystem &;
    static void destroy();

public:
    void _tick_(float dt);
    void _register_(unsigned int ID, std::shared_ptr<PhysicsObject> ptr);

public:
    PhysicsSystem(const PhysicsSystem &) = delete;
    auto operator=(const PhysicsSystem &) -> PhysicsSystem & = delete;
    PhysicsSystem(PhysicsSystem &&) = delete;
    auto operator=(PhysicsSystem &&) -> PhysicsSystem & = delete;
private: // disable any instantiation outside
    PhysicsSystem() : kernel(FastMassSpringKernel()) {}
    ~PhysicsSystem() = default;

private:
    friend PBDKernel;
    friend XPBDKernel;
    friend FastMassSpringKernel;
    friend SimpleCollision;
    std::variant<PBDKernel, XPBDKernel, FastMassSpringKernel> kernel;
    std::variant<SimpleCollision> collision;
    std::map<unsigned int, std::shared_ptr<PhysicsObject>> physics_objects;
    std::map<unsigned int, std::shared_ptr<PhysicsObject>> erased_physics_objects;
    std::vector<Constraint> constraints;
};
}

#endif //HINAPE_PHYSICS_SYSTEM_H
