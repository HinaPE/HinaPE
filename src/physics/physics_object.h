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
    [[nodiscard]] auto get_object() const -> const T &;
    [[nodiscard]] auto get_type() const -> PhysicsObjectType;

    // universal methods
    [[nodiscard]] auto get_position() const -> Vec3;
    [[nodiscard]] auto get_rotation() const -> Vec3;
    [[nodiscard]] auto get_velocity() const -> Vec3;
    [[nodiscard]] auto get_force() const -> Vec3;
    [[nodiscard]] auto get_mass() const -> float;
    void set_position(const Vec3 &) const;
    void set_rotation(const Vec3 &) const;
    void set_velocity(const Vec3 &) const;

    // rigidbody methods
    auto is_rigidbody() -> bool;
    [[nodiscard]] auto get_rigid_body_type() const -> RigidBodyType;
    void switch_rigidbody_type(RigidBodyType to);

    // deformable methods
    auto is_deformable() -> bool;
    [[nodiscard]] auto get_deformable_type() const -> DeformableType;
    auto pos() -> std::vector<Vec3> &;
    auto dirty_pos() -> const std::vector<Vec3> &;
    auto dirty_ind() -> const std::vector<unsigned int> &;

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

inline PhysicsObject::PhysicsObject(PhysicsObjectType type)
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
auto HinaPE::PhysicsObject::get_object() const -> const T &
{
    return std::get<T>(physics_object_opt.value());
}

}

#endif //HINAPE_PHYSICS_OBJECT_H
