#ifndef HINAPE_PHYSICS_OBJECT_H
#define HINAPE_PHYSICS_OBJECT_H

#include "physics_objects/deformable.h"
#include "physics_objects/rigidbody.h"
#include "kernels/pbd/constraints.h"

#include <vector>
#include <variant>
#include <optional>

namespace HinaPE
{
class PhysicsObject
{
public:
    // common methods
    template<class T>
    [[nodiscard]] auto get_object() -> T &;
    template<class T>
    [[nodiscard]] auto is() const -> bool;

    // universal methods
    [[nodiscard]] auto get_position() const -> Vec3;
    [[nodiscard]] auto get_rotation() const -> Vec3;
    [[nodiscard]] auto get_vertices() -> std::vector<Vec3> &;
    [[nodiscard]] auto get_indices() -> std::vector<int> &;
    auto set_position(const Vec3 &) const -> void;
    auto set_rotation(const Vec3 &) const -> void;

    // rigidbody methods
    [[nodiscard]] auto is_rigidbody() -> bool;
    [[nodiscard]] auto get_rigid_body_type() const -> RigidBodyType;
    auto switch_rigidbody_type(RigidBodyType to) -> void;

    // deformable methods
    [[nodiscard]] auto is_deformable() -> bool;
    [[nodiscard]] auto get_deformable_type() const -> DeformableType;
    [[nodiscard]] auto get_velocities() -> std::vector<Vec3> &;
    [[nodiscard]] auto get_masses() -> std::vector<float> &;
    [[nodiscard]] auto get_inv_masses() -> std::vector<float> &;

public:
    explicit PhysicsObject(PhysicsObjectType type);
    ~PhysicsObject() = default;
    PhysicsObject(const PhysicsObject &src) = delete;
    auto operator=(const PhysicsObject &src) -> PhysicsObject & = delete;
    PhysicsObject(PhysicsObject &&src) = default;
    auto operator=(PhysicsObject &&src) -> PhysicsObject & = default;

    template<RigidBodyType T>
    explicit PhysicsObject(RigidBodyBase<T> &&rigidbody);
    template<RigidBodyType T>
    auto operator=(RigidBodyBase<T> &&rigidbody) -> PhysicsObject &;
    template<DeformableType T>
    explicit PhysicsObject(DeformableBase<T> &&deformable);
    template<DeformableType T>
    auto operator=(DeformableBase<T> &&deformable) -> PhysicsObject &;

private:
    std::optional<std::variant<RigidBodyBase<HinaPE::DYNAMIC>, RigidBodyBase<HinaPE::STATIC>, RigidBodyBase<HinaPE::KINEMATIC>, DeformableBase<CLOTH>, DeformableBase<MESH>>> physics_object_opt;
    std::vector<Constraint> inner_constraints;
};

template<HinaPE::RigidBodyType T>
HinaPE::PhysicsObject::PhysicsObject(HinaPE::RigidBodyBase<T> &&rigidbody)
{
    physics_object_opt = std::move(rigidbody);
}

template<HinaPE::RigidBodyType T>
auto HinaPE::PhysicsObject::operator=(HinaPE::RigidBodyBase<T> &&rigidbody) -> HinaPE::PhysicsObject &
{
    physics_object_opt = std::move(rigidbody);
    return *this;
}

template<HinaPE::DeformableType T>
HinaPE::PhysicsObject::PhysicsObject(HinaPE::DeformableBase<T> &&deformable)
{
    physics_object_opt = std::move(deformable);
}

template<HinaPE::DeformableType T>
auto HinaPE::PhysicsObject::operator=(HinaPE::DeformableBase<T> &&deformable) -> HinaPE::PhysicsObject &
{
    physics_object_opt = std::move(deformable);
    return *this;
}

inline HinaPE::PhysicsObject::PhysicsObject(PhysicsObjectType type)
{
    switch (type)
    {
        case Rigidbody:
            physics_object_opt = RigidBodyBase<HinaPE::DYNAMIC>();
            break;
        case Deformable:
            throw std::runtime_error("DEFORMABLE NOT IMPLEMENTED");
            break;
        case Fluid:
            throw std::runtime_error("FLUID NOT IMPLEMENTED");
            break;
        case NOT_PHYSICS_OBJECT:
            physics_object_opt = std::nullopt;
            break;
    }
}

template<class T>
auto HinaPE::PhysicsObject::get_object() -> T &
{
    return *std::get_if<T>(&physics_object_opt.value());
}

template<class T>
auto PhysicsObject::is() const -> bool
{
    if (!physics_object_opt.has_value())
        return false;

    if (std::get_if<T>(&physics_object_opt.value()))
        return true;
    return false;
}

}

#endif //HINAPE_PHYSICS_OBJECT_H
