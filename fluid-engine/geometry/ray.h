#ifndef HINAPE_FLUID_ENGINE_RAY_H_
#define HINAPE_FLUID_ENGINE_RAY_H_

#include "math/vector.h"

namespace HinaPE::FluidEngine
{

//!
//! \brief      Class for ray.
//!
//! \tparam     T     The value type.
//! \tparam     N     The dimension.
//!
template<typename T, size_t N>
class Ray
{
    static_assert(N != 2 && N != 3, "Not implemented.");
    static_assert(std::is_floating_point<T>::value, "Ray only can be instantiated with floating point types");
};

}  // namespace HinaPE::FluidEngine

#endif  // HINAPE_FLUID_ENGINE_RAY_H_
