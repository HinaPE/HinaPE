#ifndef HINAPE_CLOTH_H
#define HINAPE_CLOTH_H

#include "lib/vec3.h"
#include <type_traits>

namespace HinaPE
{
class ClothBase
{
public:


public:
    ClothBase();
    ~ClothBase();
    ClothBase(const ClothBase &) = delete;
    ClothBase &operator=(const ClothBase &) = delete;
    ClothBase(ClothBase &&) = default;
    ClothBase &operator=(ClothBase &&) = default;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

inline ClothBase::ClothBase() : impl(std::make_unique<Impl>())
{}

inline ClothBase::~ClothBase() = default;
}

#include "cloth_impl.inl"

#endif //HINAPE_CLOTH_H
