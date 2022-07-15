#include "pbd_kernel.h"
#include "../../physics_system.h"

void HinaPE::PBDKernel::simulate(HinaPE::PhysicsSystem &sys, float dt)
{
    auto &os = sys.physics_objects;

    for (auto &pair: os)
    {
        auto &o = pair.second;
        auto type = o->get_type();
        switch (type)
        {
            case Rigidbody:
            {
                auto p = o->get_position();
                auto q = o->get_rotation();
                auto v = o->get_velocity();
                auto m = o->get_mass();
                auto f = o->get_force();
                auto a = Vec3(0.f, -9.8f, 0.f);
                TimeIntegration::semi_implicit_euler(dt, m, p, v, a);
                /*
                 * TODO: Implement more physically
                 */
                o->set_position(p);
                if (p.y < -3.f)
                    o->set_velocity(-v);
                else
                    o->set_velocity(v);
            }
                break;
            case Deformable:
                throw std::runtime_error("Deformable Not Implemented");
            case Fluid:
                throw std::runtime_error("Fluid Not Implemented");
            case NOT_PHYSICS_OBJECT:
                break;
            default:
                throw std::runtime_error("invalid Physics Object type");
        }
    }
}
