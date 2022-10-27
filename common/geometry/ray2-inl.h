#ifndef HINAPE_FLUID_ENGINE_DETAIL_RAY2_INL_H_
#define HINAPE_FLUID_ENGINE_DETAIL_RAY2_INL_H_

#include "ray2.h"

namespace HinaPE::FluidEngine
{

template<typename T>
Ray<T, 2>::Ray() : Ray(Vector2<T>(), Vector2<T>(1, 0))
{
}

template<typename T>
Ray<T, 2>::Ray(const Vector2<T> &newOrigin, const Vector2<T> &newDirection) :
        origin(newOrigin), direction(newDirection.normalized())
{
}

template<typename T>
Ray<T, 2>::Ray(const Ray &other) :
        origin(other.origin), direction(other.direction)
{
}

template<typename T>
Vector2<T> Ray<T, 2>::pointAt(T t) const
{
    return origin + t * direction;
}

}  // namespace HinaPE::FluidEngine

#endif  // HINAPE_FLUID_ENGINE_DETAIL_RAY2_INL_H_
