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

    [[nodiscard]] Vec3 get_position() const;
    [[nodiscard]] Vec3 get_rotation() const;
    void set_position(const Vec3 &) const;
    void set_rotation(const Vec3 &) const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<(T == DYNAMIC || T == KINEMATIC)>::type>
    [[nodiscard]] float get_mass() const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] Vec3 get_force() const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] Vec3 get_linear_velocity() const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] Vec3 get_angular_velocity() const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] float get_linear_damping() const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] float get_angular_damping() const;

    template<RigidBodyType T = Type, typename = typename std::enable_if<(T == DYNAMIC || T == KINEMATIC)>::type>
    void set_mass(float m) const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    void set_linear_velocity(const Vec3 &v) const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    void add_force(const Vec3 &f) const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    void add_acceleration(const Vec3 &a) const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    void set_angular_velocity(const Vec3 &w) const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    void set_linear_damping(float d) const;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    void set_angular_damping(float d) const;

public:
    RigidBodyBase();
    ~RigidBodyBase();
    RigidBodyBase(const RigidBodyBase &src) = delete;
    RigidBodyBase &operator=(const RigidBodyBase &src) = delete;
    RigidBodyBase(RigidBodyBase &&src) noexcept;
    RigidBodyBase &operator=(RigidBodyBase &&src) noexcept;

private:
    template<RigidBodyType FromType, RigidBodyType ResType>
    friend RigidBodyBase<ResType> switch_rigidbody_type(const RigidBodyBase<FromType> &from);
    template<RigidBodyType FromType, RigidBodyType ResType>
    friend void copy_impl(typename RigidBodyBase<FromType>::Impl *from, typename RigidBodyBase<ResType>::Impl *res);

    friend class RigidBodyFactory;
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
