#include "physics_object.h"

#include "lib/mathlib.h"

auto HinaPE::PhysicsObject::get_type() const -> HinaPE::PhysicsObjectType
{
    if (!physics_object_opt.has_value())
        return NOT_PHYSICS_OBJECT;
    switch (physics_object_opt.value().index())
    {
        case 0: // RigidBodyBase<DYNAMIC>
        case 1: // RigidBodyBase<STATIC>
        case 2: // RigidBodyBase<KINEMATIC>
            return Rigidbody;
        case 3: // DeformableBase<CLOTH>
        case 4: // DeformableBase<MESH>
            return Deformable;
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}

auto HinaPE::PhysicsObject::get_position() const -> Vec3
{
    return std::visit([&](auto &o) -> Vec3
                      {
                          return o.get_position();
                      }, physics_object_opt.value());
}

auto HinaPE::PhysicsObject::get_rotation() const -> Vec3
{
    return std::visit([&](auto &o) -> Vec3
                      {
                          return o.get_rotation();
                      }, physics_object_opt.value());
}

auto HinaPE::PhysicsObject::get_vertices() -> std::vector<Vec3> &
{
    switch (physics_object_opt.value().index())
    {
        case 0: // RigidBodyBase<DYNAMIC>
        case 1: // RigidBodyBase<STATIC>
        case 2: // RigidBodyBase<KINEMATIC>
            throw std::runtime_error("rigidbody geometry not implemented"); // TODO: to support in the future
        case 3: // DeformableBase<CLOTH>
            return std::get<HinaPE::DeformableBase<CLOTH>>(physics_object_opt.value()).vertices();
        case 4: // DeformableBase<MESH>
            return std::get<HinaPE::DeformableBase<MESH>>(physics_object_opt.value()).vertices();
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}

auto HinaPE::PhysicsObject::get_indices() -> std::vector<unsigned int> &
{
    switch (physics_object_opt.value().index())
    {
        case 0: // RigidBodyBase<DYNAMIC>
        case 1: // RigidBodyBase<STATIC>
        case 2: // RigidBodyBase<KINEMATIC>
            throw std::runtime_error("rigidbody geometry not implemented"); // TODO: to support in the future
        case 3: // DeformableBase<CLOTH>
            return std::get<HinaPE::DeformableBase<CLOTH>>(physics_object_opt.value()).indices();
        case 4: // DeformableBase<MESH>
            return std::get<HinaPE::DeformableBase<MESH>>(physics_object_opt.value()).indices();
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}

auto HinaPE::PhysicsObject::set_position(const Vec3 &p) const -> void
{
    std::visit([&](auto &o)
               {
                   o.set_position(p);
               }, physics_object_opt.value());
}

auto HinaPE::PhysicsObject::set_rotation(const Vec3 &r) const -> void
{
    std::visit([&](auto &o)
               {
                   o.set_rotation(r);
               }, physics_object_opt.value());
}

auto HinaPE::PhysicsObject::switch_rigidbody_type(HinaPE::RigidBodyType to) -> void
{
    if (physics_object_opt == std::nullopt)
        switch (to)
        {
            case HinaPE::DYNAMIC:
                physics_object_opt = HinaPE::RigidBodyBase<HinaPE::DYNAMIC>();
                break;
            case HinaPE::STATIC:
                physics_object_opt = HinaPE::RigidBodyBase<HinaPE::STATIC>();
                break;
            case HinaPE::KINEMATIC:
                physics_object_opt = HinaPE::RigidBodyBase<HinaPE::KINEMATIC>();
                break;
            default:
                throw std::runtime_error("invalid rigidbody type");
        }
    else
        std::visit(overloaded{[&](HinaPE::RigidBodyBase<HinaPE::DYNAMIC> &dyn)
                              {
                                  switch (to)
                                  {
                                      case HinaPE::NOT_RIGIDBODY:
                                          physics_object_opt = std::nullopt;
                                          break;
                                      case HinaPE::STATIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::DYNAMIC, HinaPE::STATIC>(std::get<HinaPE::RigidBodyBase<HinaPE::DYNAMIC>>(physics_object_opt.value()));
                                          break;
                                      case HinaPE::KINEMATIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::DYNAMIC, HinaPE::KINEMATIC>(std::get<HinaPE::RigidBodyBase<HinaPE::DYNAMIC>>(physics_object_opt.value()));
                                          break;
                                      default:
                                          throw std::runtime_error("invalid rigidbody type");
                                  }
                              }, [&](HinaPE::RigidBodyBase<HinaPE::STATIC> &sta)
                              {
                                  switch (to)
                                  {
                                      case HinaPE::NOT_RIGIDBODY:
                                          physics_object_opt = std::nullopt;
                                          break;
                                      case HinaPE::DYNAMIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::STATIC, HinaPE::DYNAMIC>(std::get<HinaPE::RigidBodyBase<HinaPE::STATIC>>(physics_object_opt.value()));
                                          break;
                                      case HinaPE::KINEMATIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::STATIC, HinaPE::KINEMATIC>(std::get<HinaPE::RigidBodyBase<HinaPE::STATIC>>(physics_object_opt.value()));
                                          break;
                                      default:
                                          throw std::runtime_error("invalid rigidbody type");
                                  }
                              }, [&](HinaPE::RigidBodyBase<HinaPE::KINEMATIC> &kin)
                              {
                                  switch (to)
                                  {
                                      case HinaPE::NOT_RIGIDBODY:
                                          physics_object_opt = std::nullopt;
                                          break;
                                      case HinaPE::DYNAMIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::KINEMATIC, HinaPE::DYNAMIC>(std::get<HinaPE::RigidBodyBase<HinaPE::KINEMATIC>>(physics_object_opt.value()));
                                          break;
                                      case HinaPE::STATIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::KINEMATIC, HinaPE::STATIC>(std::get<HinaPE::RigidBodyBase<HinaPE::KINEMATIC>>(physics_object_opt.value()));
                                          break;
                                      default:
                                          throw std::runtime_error("invalid rigidbody type");
                                  }
                              }, [&](HinaPE::DeformableBase<CLOTH> &cloth) { throw std::runtime_error("invalid rigidbody type"); }, [&](HinaPE::DeformableBase<MESH> &mesh) { throw std::runtime_error("invalid rigidbody type"); }}, physics_object_opt.value());
}

auto HinaPE::PhysicsObject::is_rigidbody() -> bool
{
    return physics_object_opt != std::nullopt && physics_object_opt->index() < 3;
}

auto HinaPE::PhysicsObject::get_rigid_body_type() const -> HinaPE::RigidBodyType
{
    if (!physics_object_opt.has_value())
        throw std::runtime_error("physics object is not rigidbody");
    switch (physics_object_opt.value().index())
    {
        case 0:
            return DYNAMIC;
        case 1:
            return STATIC;
        case 2:
            return KINEMATIC;
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}

auto HinaPE::PhysicsObject::is_deformable() -> bool
{
    return physics_object_opt != std::nullopt && physics_object_opt->index() == 3;
}

auto HinaPE::PhysicsObject::get_deformable_type() const -> HinaPE::DeformableType
{
    if (!physics_object_opt.has_value())
        throw std::runtime_error("physics object is not rigidbody");
    switch (physics_object_opt.value().index())
    {
        case 3:
            return CLOTH;
        case 4:
            return MESH;
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}

auto HinaPE::PhysicsObject::get_velocities() -> std::vector<Vec3> &
{
    switch (physics_object_opt.value().index())
    {
        case 0: // RigidBodyBase<DYNAMIC>
        case 1: // RigidBodyBase<STATIC>
        case 2: // RigidBodyBase<KINEMATIC>
            throw std::runtime_error("use get velocity for rigid body"); // TODO: to support in the future
        case 3: // DeformableBase<CLOTH>
            return std::get<HinaPE::DeformableBase<CLOTH>>(physics_object_opt.value()).velocities();
        case 4: // DeformableBase<MESH>
            return std::get<HinaPE::DeformableBase<MESH>>(physics_object_opt.value()).velocities();
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}

auto HinaPE::PhysicsObject::get_masses() -> std::vector<float> &
{
    switch (physics_object_opt.value().index())
    {
        case 0: // RigidBodyBase<DYNAMIC>
        case 1: // RigidBodyBase<STATIC>
        case 2: // RigidBodyBase<KINEMATIC>
            throw std::runtime_error("use get mass for rigid body"); // TODO: to support in the future
        case 3: // DeformableBase<CLOTH>
            return std::get<HinaPE::DeformableBase<CLOTH>>(physics_object_opt.value()).masses();
        case 4: // DeformableBase<MESH>
            return std::get<HinaPE::DeformableBase<MESH>>(physics_object_opt.value()).masses();
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}
auto HinaPE::PhysicsObject::get_inv_masses() -> std::vector<float> &
{
    switch (physics_object_opt.value().index())
    {
        case 0: // RigidBodyBase<DYNAMIC>
        case 1: // RigidBodyBase<STATIC>
        case 2: // RigidBodyBase<KINEMATIC>
            throw std::runtime_error("use get inv mass for rigid body"); // TODO: to support in the future
        case 3: // DeformableBase<CLOTH>
            return std::get<HinaPE::DeformableBase<CLOTH>>(physics_object_opt.value()).inv_masses();
        case 4: // DeformableBase<MESH>
            return std::get<HinaPE::DeformableBase<MESH>>(physics_object_opt.value()).inv_masses();
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}