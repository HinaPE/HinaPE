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
    [[nodiscard]] Vec3 get_position() const;
    [[nodiscard]] Vec3 get_rotation() const;
    void set_position(const Vec3 &) const;
    void set_rotation(const Vec3 &) const;

    std::vector<Vec3> &pos();
    const std::vector<Vec3> &dirty_pos();
    const std::vector<unsigned int> &dirty_ind();

public:
    DeformableBase();
    ~DeformableBase();
    DeformableBase(const DeformableBase &) = delete;
    DeformableBase &operator=(const DeformableBase &) = delete;
    DeformableBase(DeformableBase &&) noexcept = default;
    DeformableBase &operator=(DeformableBase &&) noexcept = default;

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
