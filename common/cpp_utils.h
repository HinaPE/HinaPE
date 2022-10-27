// Copyright (c) 2018 Doyub Kim
//
// I am making my contributions/submissions to this project solely in my
// personal capacity and am not conveying any rights to any intellectual
// property of any third parties.

#ifndef HINAPE_FLUID_ENGINE_CPP_UTILS_H_
#define HINAPE_FLUID_ENGINE_CPP_UTILS_H_

#include <algorithm>

namespace HinaPE::FluidEngine
{

template<class ForwardIt, class T, class Compare = std::less<T>>
ForwardIt binaryFind(ForwardIt first, ForwardIt last, const T &value, Compare comp = {});
}  // namespace HinaPE::FluidEngine

#include "cpp_utils-inl.h"

#endif  // HINAPE_FLUID_ENGINE_CPP_UTILS_H_
