#ifndef HINAPE_DEFORMABLE_H
#define HINAPE_DEFORMABLE_H

#include "../common.h"

#include <memory>
#include <type_traits>

namespace HinaPE
{
template<DeformableType Type>
class DeformableBase
{
public:
    [[nodiscard]] auto get_position() const -> Vec3;
    [[nodiscard]] auto get_rotation() const -> Vec3;
    auto set_position(const Vec3 &) const -> void;
    auto set_rotation(const Vec3 &) const -> void;

    auto pos() -> std::vector<Vec3> &;
    auto ind() -> std::vector<unsigned int> &;
    auto vel() -> std::vector<Vec3> &;

public:
    DeformableBase();
    ~DeformableBase();
    DeformableBase(const DeformableBase &) = delete;
    auto operator=(const DeformableBase &) -> DeformableBase & = delete;
    DeformableBase(DeformableBase &&) noexcept = default;
    auto operator=(DeformableBase &&) noexcept -> DeformableBase & = default;

private:
    friend class ClothFactory;
    struct Impl;
    std::unique_ptr<Impl> impl;
};

template<DeformableType Type>
DeformableBase<Type>::DeformableBase() : impl(std::make_unique<Impl>()) {}

template<DeformableType Type>
DeformableBase<Type>::~DeformableBase() = default;
}

#include "deformable_impl.inl"

#endif //HINAPE_DEFORMABLE_H
