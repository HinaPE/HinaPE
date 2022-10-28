#ifndef HINAPE_FLUID_ENGINE_DETAIL_SAMPLERS_INL_H_
#define HINAPE_FLUID_ENGINE_DETAIL_SAMPLERS_INL_H_

#include "constants.h"
#include "math_utils.h"
#include <algorithm>

namespace HinaPE
{

template<typename T>
inline Vector3<T> uniformSampleCone(T u1, T u2, const Vector3<T> &axis, T angle)
{
    T cosAngle_2 = std::cos(angle / 2);
    T y = 1 - (1 - cosAngle_2) * u1;
    T r = std::sqrt(std::max<T>(0, 1 - y * y));
    T phi = twoPi<T>() * u2;
    T x = r * std::cos(phi);
    T z = r * std::sin(phi);
    auto ts = axis.tangential();
    return std::get<0>(ts) * x + axis * y + std::get<1>(ts) * z;
}

template<typename T>
inline Vector3<T> uniformSampleHemisphere(T u1, T u2, const Vector3<T> &normal)
{
    T y = u1;
    T r = std::sqrt(std::max<T>(0, 1 - y * y));
    T phi = twoPi<T>() * u2;
    T x = r * std::cos(phi);
    T z = r * std::sin(phi);
    auto ts = normal.tangential();
    return std::get<0>(ts) * x + normal * y + std::get<1>(ts) * z;
}

template<typename T>
inline Vector3<T> cosineWeightedSampleHemisphere(T u1, T u2, const Vector3<T> &normal)
{
    T phi = twoPi<T>() * u1;
    T y = std::sqrt(u2);
    T theta = std::acos(y);
    T x = std::cos(phi) * std::sin(theta);
    T z = std::sin(phi) * std::sin(theta);
    Vector3<T> t = tangential(normal);
    auto ts = normal.tangential();
    return std::get<0>(ts) * x + normal * y + std::get<1>(ts) * z;
}

template<typename T>
inline Vector3<T> uniformSampleSphere(T u1, T u2)
{
    T y = 1 - 2 * u1;
    T r = std::sqrt(std::max<T>(0, 1 - y * y));
    T phi = twoPi<T>() * u2;
    T x = r * std::cos(phi);
    T z = r * std::sin(phi);
    return Vector3<T>(x, y, z);
}

template<typename T>
inline Vector2<T> uniformSampleDisk(T u1, T u2)
{
    T r = std::sqrt(u1);
    T theta = twoPi<T>() * u2;

    return Vector2<T>(r * std::cos(theta), r * std::sin(theta));
}

}  // namespace HinaPE

#endif  // HINAPE_FLUID_ENGINE_DETAIL_SAMPLERS_INL_H_
