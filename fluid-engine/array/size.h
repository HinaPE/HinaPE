#ifndef HINAPE_FLUID_ENGINE_SIZE_H_
#define HINAPE_FLUID_ENGINE_SIZE_H_

#include "geometry/point.h"

namespace HinaPE::FluidEngine
{

//! \brief N-D size type.
template<size_t N> using Size = Point<size_t, N>;

}  // namespace HinaPE::FluidEngine

// #include "size-inl.h"

#endif  // HINAPE_FLUID_ENGINE_SIZE_H_

