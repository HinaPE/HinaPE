#include "physics_object.h"

#include "lib/mathlib.h"

HinaPE::PhysicsObjectType HinaPE::PhysicsObject::get_type() const
{
    if (!physics_object_opt.has_value())
        return NOT_PHYSICS_OBJECT;
    switch (physics_object_opt.value().index())
    {
        case 0:
        case 1:
        case 2:
            return Rigidbody;
        case 3:
            return Deformable;
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}

Vec3 HinaPE::PhysicsObject::get_position() const
{
    return std::visit([&](auto &o) -> Vec3
                      {
                          return o.get_position();
                      }, physics_object_opt.value());
}

Vec3 HinaPE::PhysicsObject::get_rotation() const
{
    return std::visit([&](auto &o) -> Vec3
                      {
                          return o.get_rotation();
                      }, physics_object_opt.value());
}

Vec3 HinaPE::PhysicsObject::get_velocity() const
{
    return std::visit(overloaded{[&](const RigidBodyBase<DYNAMIC> &rb) -> Vec3 { return rb.get_linear_velocity(); },
                                 [&](const RigidBodyBase<STATIC> &rb) -> Vec3 { throw std::runtime_error("DO NOT GET VELOCITY FOR STATIC RIGIDBODY"); },
                                 [&](const RigidBodyBase<KINEMATIC> &rb) -> Vec3 { throw std::runtime_error("DO NOT GET VELOCITY FOR KINEMATIC RIGIDBODY"); },
                                 [&](const HinaPE::DeformableBase<CLOTH> &cloth) -> Vec3 { throw std::runtime_error("DO NOT GET VELOCITY FOR CLOTH"); },
                                 [&](const HinaPE::DeformableBase<MESH> &mesh) -> Vec3 { throw std::runtime_error("DO NOT GET VELOCITY FOR MESH"); }},
                      physics_object_opt.value());
}

Vec3 HinaPE::PhysicsObject::get_force() const
{
    return std::visit(overloaded{[&](const RigidBodyBase<DYNAMIC> &rb) -> Vec3 { return rb.get_force(); },
                                 [&](const RigidBodyBase<STATIC> &rb) -> Vec3 { throw std::runtime_error("DO NOT GET FORCE FOR STATIC RIGIDBODY"); },
                                 [&](const RigidBodyBase<KINEMATIC> &rb) -> Vec3 { throw std::runtime_error("DO NOT GET FORCE FOR KINEMATIC RIGIDBODY"); },
                                 [&](const HinaPE::DeformableBase<CLOTH> &cloth) -> Vec3 { throw std::runtime_error("DO NOT GET FORCE FOR CLOTH"); },
                                 [&](const HinaPE::DeformableBase<MESH> &mesh) -> Vec3 { throw std::runtime_error("DO NOT GET FORCE FOR MESH"); }},
                      physics_object_opt.value());
}

float HinaPE::PhysicsObject::get_mass() const
{
    return std::visit(overloaded{[&](const RigidBodyBase<DYNAMIC> &rb) -> float { return rb.get_mass(); },
                                 [&](const RigidBodyBase<STATIC> &rb) -> float { throw std::runtime_error("DO NOT GET MASS FOR STATIC RIGIDBODY"); },
                                 [&](const RigidBodyBase<KINEMATIC> &rb) -> float { return rb.get_mass(); },
                                 [&](const HinaPE::DeformableBase<CLOTH> &cloth) -> float { throw std::runtime_error("DO NOT GET MASS FOR CLOTH"); },
                                 [&](const HinaPE::DeformableBase<MESH> &mesh) -> float { throw std::runtime_error("DO NOT GET MASS FOR MESH"); }},
                      physics_object_opt.value());
}

void HinaPE::PhysicsObject::set_position(const Vec3 &p) const
{
    std::visit([&](auto &o)
               {
                   o.set_position(p);
               }, physics_object_opt.value());
}

void HinaPE::PhysicsObject::set_rotation(const Vec3 &r) const
{
    std::visit([&](auto &o)
               {
                   o.set_rotation(r);
               }, physics_object_opt.value());
}

void HinaPE::PhysicsObject::set_velocity(const Vec3 &v) const
{
    std::visit(overloaded{[&](const RigidBodyBase<DYNAMIC> &rb) { rb.set_linear_velocity(v); },
                          [&](const RigidBodyBase<STATIC> &rb) { throw std::runtime_error("DO NOT SET VELOCITY FOR STATIC RIGIDBODY"); },
                          [&](const RigidBodyBase<KINEMATIC> &rb) { throw std::runtime_error("DO NOT SET VELOCITY FOR STATIC RIGIDBODY"); },
                          [&](const HinaPE::DeformableBase<CLOTH> &cloth) { throw std::runtime_error("DO NOT SET VELOCITY FOR CLOTH"); },
                          [&](const HinaPE::DeformableBase<MESH> &mesh) { throw std::runtime_error("DO NOT SET VELOCITY FOR MESH"); }}, physics_object_opt.value());
}

void HinaPE::PhysicsObject::switch_rigidbody_type(HinaPE::RigidBodyType to)
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
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::DYNAMIC, HinaPE::STATIC>(
                                                  std::get<HinaPE::RigidBodyBase<HinaPE::DYNAMIC>>(physics_object_opt.value()));
                                          break;
                                      case HinaPE::KINEMATIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::DYNAMIC, HinaPE::KINEMATIC>(
                                                  std::get<HinaPE::RigidBodyBase<HinaPE::DYNAMIC>>(physics_object_opt.value()));
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
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::STATIC, HinaPE::DYNAMIC>(
                                                  std::get<HinaPE::RigidBodyBase<HinaPE::STATIC>>(physics_object_opt.value()));
                                          break;
                                      case HinaPE::KINEMATIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::STATIC, HinaPE::KINEMATIC>(
                                                  std::get<HinaPE::RigidBodyBase<HinaPE::STATIC>>(physics_object_opt.value()));
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
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::KINEMATIC, HinaPE::DYNAMIC>(
                                                  std::get<HinaPE::RigidBodyBase<HinaPE::KINEMATIC>>(physics_object_opt.value()));
                                          break;
                                      case HinaPE::STATIC:
                                          physics_object_opt = HinaPE::switch_rigidbody_type<HinaPE::KINEMATIC, HinaPE::STATIC>(
                                                  std::get<HinaPE::RigidBodyBase<HinaPE::KINEMATIC>>(physics_object_opt.value()));
                                          break;
                                      default:
                                          throw std::runtime_error("invalid rigidbody type");
                                  }
                              }, [&](HinaPE::DeformableBase<CLOTH> &cloth) { throw std::runtime_error("invalid rigidbody type"); },
                              [&](HinaPE::DeformableBase<MESH> &mesh) { throw std::runtime_error("invalid rigidbody type"); }}, physics_object_opt.value());
}

bool HinaPE::PhysicsObject::is_rigidbody()
{
    return physics_object_opt != std::nullopt && physics_object_opt->index() < 3;
}

HinaPE::RigidBodyType HinaPE::PhysicsObject::get_rigid_body_type() const
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

bool HinaPE::PhysicsObject::is_deformable()
{
    return physics_object_opt != std::nullopt && physics_object_opt->index() == 3;
}

HinaPE::DeformableType HinaPE::PhysicsObject::get_deformable_type() const
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

const std::vector<Vec3> &HinaPE::PhysicsObject::dirty_pos()
{
    static std::vector<Vec3> null_vector;

    switch (physics_object_opt.value().index())
    {
        case 0: // RigidBodyBase<DYNAMIC>
        case 1: // RigidBodyBase<STATIC>
        case 2: // RigidBodyBase<KINEMATIC>
            return null_vector;
        case 3: // DeformableBase<CLOTH>
            return std::get<HinaPE::DeformableBase<CLOTH>>(physics_object_opt.value()).dirty_pos();
        case 4: // DeformableBase<MESH>
            return std::get<HinaPE::DeformableBase<MESH>>(physics_object_opt.value()).dirty_pos();
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}

const std::vector<unsigned int> &HinaPE::PhysicsObject::dirty_ind()
{
    static std::vector<unsigned int> null_vector;

    switch (physics_object_opt.value().index())
    {
        case 0: // RigidBodyBase<DYNAMIC>
        case 1: // RigidBodyBase<STATIC>
        case 2: // RigidBodyBase<KINEMATIC>
            return null_vector;
        case 3: // DeformableBase<CLOTH>
            return std::get<HinaPE::DeformableBase<CLOTH>>(physics_object_opt.value()).dirty_ind();
        case 4: // DeformableBase<MESH>
            return std::get<HinaPE::DeformableBase<MESH>>(physics_object_opt.value()).dirty_ind();
        default:
            throw std::runtime_error("invalid rigidbody type");
    }
}
