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
                semi_implicit_euler(dt, m, p, v, a);
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
            {

            }
                break;
            case Fluid:
                throw std::runtime_error("Fluid Not Implemented");
            case NOT_PHYSICS_OBJECT:
                break;
            default:
                throw std::runtime_error("invalid Physics Object type");
        }
    }
}
void HinaPE::PBDKernel::init(const HinaPE::PBDKernel::Opt &opt)
{

}
void HinaPE::PBDKernel::prediction(HinaPE::PhysicsObject &o)
{
    auto type = o.get_type();
    switch (type)
    {
        case Rigidbody:
        {
            auto p = o.get_position();
            auto q = o.get_rotation();
            auto v = o.get_velocity();
            auto m = o.get_mass();
            auto f = o.get_force();
            auto a = Vec3(0.f, -9.8f, 0.f);
            semi_implicit_euler(opt.dt, m, p, v, a);
            /*
             * TODO: Implement more physically
             */
            o.set_position(p);
            if (p.y < -3.f)
                o.set_velocity(-v);
            else
                o.set_velocity(v);
        }
            break;
        case Deformable:
        {

        }
            break;
        case Fluid:
            throw std::runtime_error("Fluid Not Implemented");
        case NOT_PHYSICS_OBJECT:
            break;
        default:
            throw std::runtime_error("invalid Physics Object type");
    }
}
void HinaPE::PBDKernel::collision_detection()
{

}
void HinaPE::PBDKernel::project_constraint()
{

}
void HinaPE::PBDKernel::update_states()
{

}
void HinaPE::PBDKernel::semi_implicit_euler(HinaPE::PBDKernel::real h, HinaPE::PBDKernel::real mass, Vec3 &position, Vec3 &velocity, const Vec3 &acceleration)
{
    if (mass != 0.0)
    {
        velocity += acceleration * h;
        position += velocity * h;
    }
}

