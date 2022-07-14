#ifndef HINAPE_PHYSICS_OBJECT_H
#define HINAPE_PHYSICS_OBJECT_H

#include "physics_objects/cloth.h"
#include "physics_objects/rigidbody.h"

#include <map>
#include <variant>
#include <optional>

namespace HinaPE
{
class PhysicsObject
{
public:
    void switch_rigidbody_type(RigidBodyType to);
    [[nodiscard]] bool is_physics_object() const;
    [[nodiscard]] bool is_rigidbody_dyn() const;
    [[nodiscard]] const RigidBodyBase<HinaPE::DYNAMIC> &get_rigidbody_dyn() const;
    [[nodiscard]] const RigidBodyBase<HinaPE::STATIC> &get_rigidbody_st() const;
    [[nodiscard]] const RigidBodyBase<HinaPE::KINEMATIC> &get_rigidbody_kin() const;
    [[nodiscard]] RigidBodyBase<HinaPE::DYNAMIC> &get_rigidbody_dyn();
    [[nodiscard]] RigidBodyBase<HinaPE::STATIC> &get_rigidbody_st();
    [[nodiscard]] RigidBodyBase<HinaPE::KINEMATIC> &get_rigidbody_kin();

    void doing()
    {
        if (physics_object_opt.has_value())
        {
            if (physics_object_opt.value().index() == 0)
            {
                get_rigidbody_dyn().test();
            }
        }
    }

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
