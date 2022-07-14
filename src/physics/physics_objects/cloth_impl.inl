#include "cloth.h"

namespace HinaPE
{

struct ClothBase::Impl
{
    std::vector<Vec3> nodes;

    Impl() = default;
    ~Impl() = default;
    Impl(const Impl &src) = delete;
    Impl &operator=(const Impl &src) = delete;
    Impl(Impl &&src) noexcept = default;
    Impl &operator=(Impl &&src) noexcept = default;
};

}
