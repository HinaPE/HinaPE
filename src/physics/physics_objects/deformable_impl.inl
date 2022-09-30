#include "deformable.h"

#include <vector>
#include <utility>

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
    std::vector<std::pair<unsigned int, unsigned int>> edges;

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
auto DeformableBase<Type>::vertices() -> std::vector<Vec3> & { return impl->positions; }

template<DeformableType Type>
auto DeformableBase<Type>::velocities() -> std::vector<Vec3> & { return impl->velocities; }

template<DeformableType Type>
auto DeformableBase<Type>::masses() -> std::vector<float> & { return impl->masses; }

template<DeformableType Type>
auto DeformableBase<Type>::inv_masses() -> std::vector<float> & { return impl->inv_masses; }

template<DeformableType Type>
auto DeformableBase<Type>::indices() -> std::vector<unsigned int> & { return impl->indices; }

template<DeformableType Type>
auto DeformableBase<Type>::edges() -> std::vector<std::pair<unsigned int, unsigned int>> & { return impl->edges; }

template<DeformableType Type>
auto DeformableBase<Type>::setup_geometry() -> void
{
    // simple impl of initiating edges
    auto &inds = impl->indices;
    auto &_edges = impl->edges;
    auto size = inds.size();

    assert(size % 3 == 0); // make sure is triangle geom mesh
    for (int i = 0; i < size; i += 3)
    {
        if (std::find(_edges.begin(), _edges.end(), std::make_pair(inds[i], inds[i + 1])) == _edges.end() && std::find(_edges.begin(), _edges.end(), std::make_pair(inds[i + 1], inds[i])) == _edges.end())
            _edges.push_back(std::make_pair(inds[i], inds[i + 1]));
        if (std::find(_edges.begin(), _edges.end(), std::make_pair(inds[i + 1], inds[i + 2])) == _edges.end() && std::find(_edges.begin(), _edges.end(), std::make_pair(inds[i + 2], inds[i + 1])) == _edges.end())
            _edges.push_back(std::make_pair(inds[i], inds[i + 1]));
        if (std::find(_edges.begin(), _edges.end(), std::make_pair(inds[i + 2], inds[i])) == _edges.end() && std::find(_edges.begin(), _edges.end(), std::make_pair(inds[i], inds[i + 2])) == _edges.end())
            _edges.push_back(std::make_pair(inds[i], inds[i + 1]));
    }

    HinaUtil::Log::info(_edges);
}
}
