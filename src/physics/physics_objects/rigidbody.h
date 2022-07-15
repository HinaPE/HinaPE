#ifndef HINAPE_RIGIDBODY_H
#define HINAPE_RIGIDBODY_H

#include "../common.h"

#include <memory>
#include <type_traits>

namespace HinaPE
{
template<RigidBodyType Type>
class RigidBodyBase
{
public:
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBodyBase<T> *add_force(const Vec3 &f);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBodyBase<T> *add_acceleration(const Vec3 &a);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBodyBase<T> *set_linear_velocity(const Vec3 &v);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBodyBase<T> *set_angular_velocity(const Vec3 &w);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBodyBase<T> *set_linear_damping(float d);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    HINA_FORCE_INLINE RigidBodyBase<T> *set_angular_damping(float d);

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE Vec3 get_linear_velocity() const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE Vec3 get_angular_velocity() const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE float get_linear_damping() const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE float get_angular_damping() const;

    [[nodiscard]] HINA_FORCE_INLINE Vec3 get_position() const;
    [[nodiscard]] HINA_FORCE_INLINE Vec3 get_rotation() const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<(T == DYNAMIC || T == KINEMATIC)>::type>
    [[nodiscard]] HINA_FORCE_INLINE float get_mass() const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] HINA_FORCE_INLINE Vec3 get_force() const;

    HINA_FORCE_INLINE void set_position(const Vec3 &) const;
    HINA_FORCE_INLINE void set_rotation(const Vec3 &) const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<(T == DYNAMIC || T == KINEMATIC)>::type>
    HINA_FORCE_INLINE RigidBodyBase<T> *set_mass(float m);

public:
    RigidBodyBase();
    ~RigidBodyBase();
    RigidBodyBase(const RigidBodyBase &src) = delete;
    RigidBodyBase &operator=(const RigidBodyBase &src) = delete;
    RigidBodyBase(RigidBodyBase &&src) noexcept;
    RigidBodyBase &operator=(RigidBodyBase &&src) noexcept;

public:
    template<RigidBodyType FromType, RigidBodyType ResType>
    friend RigidBodyBase<ResType> switch_rigidbody_type(const RigidBodyBase<FromType> &from);
    template<RigidBodyType FromType, RigidBodyType ResType>
    friend void copy_impl(typename RigidBodyBase<FromType>::Impl *from, typename RigidBodyBase<ResType>::Impl *res);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

template<RigidBodyType FromType, RigidBodyType ResType>
RigidBodyBase<ResType> switch_rigidbody_type(const RigidBodyBase<FromType> &from)
{
    RigidBodyBase<ResType> res;
    copy_impl<FromType, ResType>(from.impl.get(), res.impl.get());
    return std::move(res); // you may use "return res" also, compiler would help auto optimize it;
}

template<RigidBodyType Type>
RigidBodyBase<Type>::RigidBodyBase() : impl(std::make_unique<Impl>())
{}

template<RigidBodyType Type>
RigidBodyBase<Type>::~RigidBodyBase()
= default;

template<RigidBodyType Type>
RigidBodyBase<Type>::RigidBodyBase(RigidBodyBase &&src) noexcept
{ impl = std::move(src.impl); }

template<RigidBodyType Type>
RigidBodyBase<Type> &RigidBodyBase<Type>::operator=(RigidBodyBase &&src) noexcept
{
    impl = std::move(src.impl);
    return *this;
}

}

#include "rigidbody_impl.inl"


#endif //HINAPE_RIGIDBODY_H
