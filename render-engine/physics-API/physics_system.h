#ifndef HINAPE_PHYSICS_SYSTEM_H
#define HINAPE_PHYSICS_SYSTEM_H

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
    bool running = true;
    int sub_step = 5;

public:
    PhysicsSystem(const PhysicsSystem &) = delete;
    PhysicsSystem(PhysicsSystem &&) = delete;
    auto operator=(const PhysicsSystem &) -> PhysicsSystem & = delete;
    auto operator=(PhysicsSystem &&) -> PhysicsSystem & = delete;
private: // disable any instantiation outside
    PhysicsSystem() = default;
    ~PhysicsSystem() = default;

private:
//    std::variant<PBDKernel, XPBDKernel, FastMassSpringKernel, SPHKernel> kernel;
    std::map<const unsigned int, std::shared_ptr<PhysicsObject>> physics_objects;
    std::map<const unsigned int, std::shared_ptr<PhysicsObject>> erased_physics_objects; // don't delete object instantly, in case UNDO operation.
};
}

#endif //HINAPE_PHYSICS_SYSTEM_H
