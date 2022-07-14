#ifndef HINAPE_PHYSICS_OBJECT_H
#define HINAPE_PHYSICS_OBJECT_H

#include "cloth.h"
#include "rigidbody.h"

#include <map>
#include <variant>
#include <optional>

namespace HinaPE
{
class PhysicsObject
{
public:
    void switch_rigidbody_type(RigidBodyType to);
    bool is_physics_object() const;

public:
    explicit PhysicsObject(PhysicsObjectType type);
    ~PhysicsObject() = default;
    PhysicsObject(const PhysicsObject &src) = delete;
    PhysicsObject &operator=(const PhysicsObject &src) = delete;
    PhysicsObject(PhysicsObject &&src) = default;
    PhysicsObject &operator=(PhysicsObject &&src) = default;

private:
    std::optional<std::variant<
            RigidBodyBase<HinaPE::DYNAMIC>,
            RigidBodyBase<HinaPE::STATIC>,
            RigidBodyBase<HinaPE::KINEMATIC>>> physics_object_opt;
};

inline PhysicsObject::PhysicsObject(PhysicsObjectType type)
{
    switch (type)
    {
        case Rigidbody:
            physics_object_opt = RigidBodyBase<HinaPE::STATIC>();
            break;
        case Deformable:
            physics_object_opt = RigidBodyBase<HinaPE::DYNAMIC>(); // TODO
            break;
        case Fluid:
            physics_object_opt = RigidBodyBase<HinaPE::KINEMATIC>(); // TODO
            break;
        case NOT_PHYSICS_OBJECT:
            physics_object_opt = std::nullopt;
            break;
    }
}

}

#endif //HINAPE_PHYSICS_OBJECT_H
