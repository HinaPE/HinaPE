#ifndef HINAPE_PHYSICS_SYSTEM_H
#define HINAPE_PHYSICS_SYSTEM_H

#include "kernels/pbd/pbd_kernel.h"
#include "kernels/xpbd/xpbd_kernel.h"
#include "kernels/fast-mass-spring/fms_kernel.h"
#include "kernels/sph/sph_kernel.h"
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
    static auto destroy() -> void;

public:
    auto _start_() -> void;
    auto _pause_() -> void;
    auto _tick_(float dt) -> void;
    auto _register_(unsigned int ID, std::shared_ptr<PhysicsObject> ptr) -> void;

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
    friend SPHKernel;
    std::variant<PBDKernel, XPBDKernel, FastMassSpringKernel, SPHKernel> kernel;
    std::map<unsigned int, std::shared_ptr<PhysicsObject>> physics_objects;
    std::map<unsigned int, std::shared_ptr<PhysicsObject>> erased_physics_objects;
    std::vector<Constraint> constraints;
};
}

#endif //HINAPE_PHYSICS_SYSTEM_H
