#ifndef HINAPE_MATH_API_H
#define HINAPE_MATH_API_H

#include "../fluid-engine/math/vector3.h"

namespace HinaPE::ClothEngine
{
using real = double;
using Vector3F = HinaPE::FluidEngine::Vector3F;
using Vector3D = HinaPE::FluidEngine::Vector3D;
using Vector3 = Vector3D;

constexpr auto as_real(auto n) -> real { return static_cast<real>(n); }
constexpr auto as_real_vector3(auto v) -> Vector3 { return Vector3(static_cast<real>(v[0]), static_cast<real>(v[1]), static_cast<real>(v[2])); }
constexpr auto as_real_vector3(auto v1, auto v2, auto v3) -> Vector3 { return Vector3(static_cast<real>(v1), static_cast<real>(v2), static_cast<real>(v3)); }
}

#endif //HINAPE_MATH_API_H
