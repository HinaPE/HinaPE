#include "rigidbody.h"

namespace HinaPE
{
template<RigidBodyType Type>
struct RigidBodyBase<Type>::Impl
{
    // Transform
    Vec3 p; // position
    Quat q; // orientation
    Vec3 s; // scale

    // Physics properties
    float m = 1.f; // mass
    float im = 1.f; // inverse mass
    Vec3 v; // linear velocity
    Vec3 w; // angular velocity
    float ld = 0.f; // linear damping
    float ad = 0.f; // angular damping
    Vec3 f; // force
    Vec3 t; // torque
    Vec3 a; // acceleration

    Impl() = default;
    ~Impl() = default;
    Impl(const Impl &src) = delete;
    Impl &operator=(const Impl &src) = delete;
    Impl(Impl &&src) noexcept = default;
    Impl &operator=(Impl &&src) noexcept = default;
};

template<RigidBodyType FromType, RigidBodyType ResType>
void copy_impl(typename RigidBodyBase<FromType>::Impl *from, typename RigidBodyBase<ResType>::Impl *res)
{
    res->p = from->p;
    res->q = from->q;
    res->s = from->s;
    res->m = from->m;
    res->im = from->im;
    res->v = from->v;
    res->w = from->w;
    res->ld = from->ld;
    res->ad = from->ad;
    res->f = from->f;
    res->t = from->t;
    res->a = from->a;
}

template<RigidBodyType Type>
template<RigidBodyType T, typename>
void RigidBodyBase<Type>::add_force(const Vec3 &f) const { impl->f += f; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
void RigidBodyBase<Type>::add_acceleration(const Vec3 &a) const { impl->a = a; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
void RigidBodyBase<Type>::set_linear_velocity(const Vec3 &v) const { impl->v = v; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
void RigidBodyBase<Type>::set_angular_velocity(const Vec3 &w) const { impl->w = w; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
void RigidBodyBase<Type>::set_linear_damping(float d) const { impl->d = d; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
void RigidBodyBase<Type>::set_angular_damping(float d) const { impl->ad = d; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
void RigidBodyBase<Type>::set_mass(float m) const { impl->m = m; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
Vec3 RigidBodyBase<Type>::get_linear_velocity() const { return impl->v; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
Vec3 RigidBodyBase<Type>::get_angular_velocity() const { return impl->w; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBodyBase<Type>::get_linear_damping() const { return impl->ld; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBodyBase<Type>::get_angular_damping() const { return impl->ad; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
float RigidBodyBase<Type>::get_mass() const { return impl->m; }

template<RigidBodyType Type>
template<RigidBodyType T, typename>
Vec3 RigidBodyBase<Type>::get_force() const { return impl->f; }

template<RigidBodyType Type>
Vec3 RigidBodyBase<Type>::get_position() const { return impl->p; }

template<RigidBodyType Type>
Vec3 RigidBodyBase<Type>::get_rotation() const { return impl->q.to_euler(); }

template<RigidBodyType Type>
void RigidBodyBase<Type>::set_position(const Vec3 &_p) const { impl->p = _p; }

template<RigidBodyType Type>
void RigidBodyBase<Type>::set_rotation(const Vec3 &r) const { /** TODO: implement **/}
}
