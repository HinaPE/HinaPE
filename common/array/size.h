#ifndef HINAPE_SIZE_H_
#define HINAPE_SIZE_H_

#include "geometry/point.h"

namespace HinaPE
{

//! \brief N-D size type.
template<size_t N> using Size = Point<size_t, N>;

}  // namespace HinaPE

// #include "size-inl.h"

#endif  // HINAPE_SIZE_H_

