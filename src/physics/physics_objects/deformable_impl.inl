#include "deformable.h"

#include <vector>

namespace HinaPE
{

template<DeformableType Type>
struct DeformableBase<Type>::Impl
{
    // Transform
    Vec3 p; // position
    Quat q; // orientation
    Vec3 s; // scale

    // Deformable particles (proxy in the future)
    std::vector<Vec3> positions;
    std::vector<Vec3> velocities;
    std::vector<float> masses;
    std::vector<float> inv_masses;

    std::vector<unsigned int> indices;

    Impl() = default;
    ~Impl() = default;
    Impl(const Impl &src) = delete;
    auto operator=(const Impl &src) -> Impl & = delete;
    Impl(Impl &&src) noexcept = default;
    auto operator=(Impl &&src) noexcept -> Impl & = default;
};

template<DeformableType Type>
auto DeformableBase<Type>::get_position() const -> Vec3 { return impl->p; }

template<DeformableType Type>
auto DeformableBase<Type>::get_rotation() const -> Vec3 { return impl->q.to_euler(); }

template<DeformableType Type>
void HinaPE::DeformableBase<Type>::set_position(const Vec3 &_p) const { impl->p = _p; }

template<DeformableType Type>
void HinaPE::DeformableBase<Type>::set_rotation(const Vec3 &_r) const { /** TODO: implement **/}

template<DeformableType Type>
auto DeformableBase<Type>::pos() -> std::vector<Vec3> & { return impl->positions; }

template<DeformableType Type>
auto DeformableBase<Type>::dirty_pos() -> const std::vector<Vec3> & { return impl->positions; }

template<DeformableType Type>
auto DeformableBase<Type>::dirty_ind() -> const std::vector<unsigned int> & { return impl->indices; }

}
