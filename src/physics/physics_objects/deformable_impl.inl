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
    Impl &operator=(const Impl &src) = delete;
    Impl(Impl &&src) noexcept = default;
    Impl &operator=(Impl &&src) noexcept = default;
};

template<DeformableType Type>
Vec3 DeformableBase<Type>::get_position() const
{
    return impl->p;
}

template<DeformableType Type>
Vec3 DeformableBase<Type>::get_rotation() const
{
    return impl->q.to_euler();
}

template<DeformableType Type>
void HinaPE::DeformableBase<Type>::set_position(const Vec3 &_p) const
{ impl->p = _p; }

template<DeformableType Type>
void HinaPE::DeformableBase<Type>::set_rotation(const Vec3 &_r) const
{ /** TODO: implement **/}

template<DeformableType Type>
const std::vector<Vec3> &DeformableBase<Type>::dirty_pos()
{ return impl->positions; }

template<DeformableType Type>
const std::vector<unsigned int> &DeformableBase<Type>::dirty_ind()
{
    static std::vector<unsigned int> null_vector;
    return null_vector;
}

}
