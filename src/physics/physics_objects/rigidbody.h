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

    [[nodiscard]] auto get_position() const -> Vec3;
    [[nodiscard]] auto get_rotation() const -> Vec3;
    auto set_position(const Vec3 &) const -> void;
    auto set_rotation(const Vec3 &) const -> void;

    template<RigidBodyType T = Type, typename = typename std::enable_if<(T == DYNAMIC || T == KINEMATIC)>::type>
    [[nodiscard]] auto get_mass() const -> float;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] auto get_force() const -> Vec3;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] auto get_linear_velocity() const -> Vec3;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] auto get_angular_velocity() const -> Vec3;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] auto get_linear_damping() const -> float;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    [[nodiscard]] auto get_angular_damping() const -> float;

    template<RigidBodyType T = Type, typename = typename std::enable_if<(T == DYNAMIC || T == KINEMATIC)>::type>
    auto set_mass(float m) const-> void;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    auto set_linear_velocity(const Vec3 &v) const-> void;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    auto add_force(const Vec3 &f) const-> void;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    auto add_acceleration(const Vec3 &a) const-> void;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    auto set_angular_velocity(const Vec3 &w) const-> void;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    auto set_linear_damping(float d) const-> void;
    template<RigidBodyType T = Type, typename = typename std::enable_if<T == DYNAMIC>::type>
    auto set_angular_damping(float d) const-> void;

public:
    RigidBodyBase();
    ~RigidBodyBase();
    RigidBodyBase(const RigidBodyBase &src) = delete;
    auto operator=(const RigidBodyBase &src) -> RigidBodyBase & = delete;
    RigidBodyBase(RigidBodyBase &&src) noexcept;
    auto operator=(RigidBodyBase &&src) noexcept -> RigidBodyBase &;

private:
    template<RigidBodyType FromType, RigidBodyType ResType>
    friend auto switch_rigidbody_type(const RigidBodyBase<FromType> &from) -> RigidBodyBase<ResType>;
    template<RigidBodyType FromType, RigidBodyType ResType>
    friend void copy_impl(typename RigidBodyBase<FromType>::Impl *from, typename RigidBodyBase<ResType>::Impl *res);

    friend class RigidBodyFactory;
    struct Impl;
    std::unique_ptr<Impl> impl;
};

template<RigidBodyType FromType, RigidBodyType ResType>
auto switch_rigidbody_type(const RigidBodyBase<FromType> &from) -> RigidBodyBase<ResType>
{
    RigidBodyBase<ResType> res;
    copy_impl<FromType, ResType>(from.impl.get(), res.impl.get());
    return std::move(res); // you may use "return res" also, compiler would help auto optimize it;
}

template<RigidBodyType Type>
RigidBodyBase<Type>::RigidBodyBase() : impl(std::make_unique<Impl>()) {}

template<RigidBodyType Type>
RigidBodyBase<Type>::~RigidBodyBase() = default;

template<RigidBodyType Type>
RigidBodyBase<Type>::RigidBodyBase(RigidBodyBase &&src) noexcept { impl = std::move(src.impl); }

template<RigidBodyType Type>
auto RigidBodyBase<Type>::operator=(RigidBodyBase &&src) noexcept -> RigidBodyBase<Type> &
{
    impl = std::move(src.impl);
    return *this;
}
}

#include "rigidbody_impl.inl"

#endif //HINAPE_RIGIDBODY_H
