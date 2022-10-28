#ifndef HINAPE_FUNCTORS_H_
#define HINAPE_FUNCTORS_H_

#include <functional>

namespace HinaPE
{

//! Type casting operator.
template<typename T, typename U>
struct TypeCast
{
    constexpr U operator()(const T &a) const;
};

//! Reverse minus operator.
template<typename T>
struct RMinus
{
    constexpr T operator()(const T &a, const T &b) const;
};

//! Reverse divides operator.
template<typename T>
struct RDivides
{
    constexpr T operator()(const T &a, const T &b) const;
};
}

#include "functors-inl.h"

#endif  // HINAPE_FUNCTORS_H_
