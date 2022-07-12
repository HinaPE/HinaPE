#include "rigidbody.h"

namespace HinaPE
{
template<RigidBodyType Type>
struct RigidBody<Type>::Impl
{
    Impl() = default;
    ~Impl() = default;

    float m; // mass
    float im; // inverse mass
    float ld; // linear damping
    float ad; // angular damping
    Vec3 v; // linear velocity
    Vec3 w; // angular velocity
    Vec3 f; // force
    Vec3 t; // torque
    Vec3 a; // acceleration
    Vec3 p; // position
    Vec3 o; // orientation
};

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBody<T> *RigidBody<Type>::add_force(const Vec3 &f)
{
    impl.f += f;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBody<T> *RigidBody<Type>::add_acceleration(const Vec3 &a)
{
    impl.a = a;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBody<T> *RigidBody<Type>::set_linear_velocity(const Vec3 &v)
{
    impl.v = v;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBody<T> *RigidBody<Type>::set_angular_velocity(const Vec3 &w)
{
    impl.w = w;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBody<T> *RigidBody<Type>::set_linear_damping(float d)
{
    impl.d = d;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBody<T> *RigidBody<Type>::set_angular_damping(float d)
{
    impl.ad = d;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBody<T> *RigidBody<Type>::set_mass(float m)
{
    impl.m = m;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
Vec3 RigidBody<Type>::get_linear_velocity() const
{
    return impl.v;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
Vec3 RigidBody<Type>::get_angular_velocity() const
{
    return impl.w;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBody<Type>::get_linear_damping() const
{
    return impl.ld;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBody<Type>::get_angular_damping() const
{
    return impl->ad;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBody<Type>::get_mass()
{
    return impl->m;
}
}
