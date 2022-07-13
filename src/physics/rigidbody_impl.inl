#include "rigidbody.h"

namespace HinaPE
{
template<RigidBodyType Type>
struct RigidBodyBase<Type>::Impl
{
    // Transform
    Vec3 p; // position
//    Quat o; // orientation
    Vec3 s; // scale

    // Physics properties
    float m; // mass
    float im; // inverse mass
    Vec3 v; // linear velocity
    Vec3 w; // angular velocity
    float ld; // linear damping
    float ad; // angular damping
    Vec3 f; // force
    Vec3 t; // torque
    Vec3 a; // acceleration

    Impl() = default;
    ~Impl() = default;
};

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBodyBase<T> *RigidBodyBase<Type>::add_force(const Vec3 &f)
{
    impl.f += f;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBodyBase<T> *RigidBodyBase<Type>::add_acceleration(const Vec3 &a)
{
    impl.a = a;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBodyBase<T> *RigidBodyBase<Type>::set_linear_velocity(const Vec3 &v)
{
    impl.v = v;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBodyBase<T> *RigidBodyBase<Type>::set_angular_velocity(const Vec3 &w)
{
    impl.w = w;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBodyBase<T> *RigidBodyBase<Type>::set_linear_damping(float d)
{
    impl.d = d;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBodyBase<T> *RigidBodyBase<Type>::set_angular_damping(float d)
{
    impl.ad = d;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
RigidBodyBase<T> *RigidBodyBase<Type>::set_mass(float m)
{
    impl.m = m;
    return this;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
Vec3 RigidBodyBase<Type>::get_linear_velocity() const
{
    return impl.v;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
Vec3 RigidBodyBase<Type>::get_angular_velocity() const
{
    return impl.w;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBodyBase<Type>::get_linear_damping() const
{
    return impl.ld;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBodyBase<Type>::get_angular_damping() const
{
    return impl->ad;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBodyBase<Type>::get_mass()
{
    return impl->m;
}
}
