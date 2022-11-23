#ifndef HINAPE_FLUID_DETAIL_SPH_KERNELS3_INL_H_
#define HINAPE_FLUID_DETAIL_SPH_KERNELS3_INL_H_

#include "sph_kernels3.h"

#include "constants.h"

namespace HinaPE::Fluid
{

inline SphStdKernel3::SphStdKernel3() : h(0), h2(0), h3(0), h5(0) {}

inline SphStdKernel3::SphStdKernel3(double kernelRadius) : h(kernelRadius), h2(h * h), h3(h2 * h), h5(h2 * h3) {}

inline SphStdKernel3::SphStdKernel3(const SphStdKernel3 &other) = default;

inline auto SphStdKernel3::operator()(double distance) const -> double
{
    if (distance * distance >= h2)
    {
        return 0.0;
    } else
    {
        double x = 1.0 - distance * distance / h2;
        return 315.0 / (64.0 * kPiD * h3) * x * x * x;
    }
}

inline auto SphStdKernel3::firstDerivative(double distance) const -> double
{
    if (distance >= h)
    {
        return 0.0;
    } else
    {
        double x = 1.0 - distance * distance / h2;
        return -945.0 / (32.0 * kPiD * h5) * distance * x * x;
    }
}

inline auto SphStdKernel3::gradient(const Vector3D &point) const -> Vector3D
{
    double dist = point.length();
    if (dist > 0.0)
    {
        return gradient(dist, point / dist);
    } else
    {
        return {0, 0, 0};
    }
}

inline auto SphStdKernel3::gradient(double distance, const Vector3D &directionToCenter) const -> Vector3D
{
    return -firstDerivative(distance) * directionToCenter;
}

inline auto SphStdKernel3::secondDerivative(double distance) const -> double
{
    if (distance * distance >= h2)
    {
        return 0.0;
    } else
    {
        double x = distance * distance / h2;
        return 945.0 / (32.0 * kPiD * h5) * (1 - x) * (5 * x - 1);
    }
}

inline SphSpikyKernel3::SphSpikyKernel3() : h(0), h2(0), h3(0), h4(0), h5(0) {}

inline SphSpikyKernel3::SphSpikyKernel3(double h_) : h(h_), h2(h * h), h3(h2 * h), h4(h2 * h2), h5(h3 * h2) {}

inline SphSpikyKernel3::SphSpikyKernel3(const SphSpikyKernel3 &other) = default;

inline auto SphSpikyKernel3::operator()(double distance) const -> double
{
    if (distance >= h)
    {
        return 0.0;
    } else
    {
        double x = 1.0 - distance / h;
        return 15.0 / (kPiD * h3) * x * x * x;
    }
}

inline auto SphSpikyKernel3::firstDerivative(double distance) const -> double
{
    if (distance >= h)
    {
        return 0.0;
    } else
    {
        double x = 1.0 - distance / h;
        return -45.0 / (kPiD * h4) * x * x;
    }
}

inline auto SphSpikyKernel3::gradient(const Vector3D &point) const -> Vector3D
{
    double dist = point.length();
    if (dist > 0.0)
    {
        return gradient(dist, point / dist);
    } else
    {
        return {0, 0, 0};
    }
}

inline auto SphSpikyKernel3::gradient(double distance, const Vector3D &directionToCenter) const -> Vector3D
{
    return -firstDerivative(distance) * directionToCenter;
}

inline auto SphSpikyKernel3::secondDerivative(double distance) const -> double
{
    if (distance >= h)
    {
        return 0.0;
    } else
    {
        double x = 1.0 - distance / h;
        return 90.0 / (kPiD * h5) * x;
    }
}

}  // namespace HinaPE::Fluid

#endif  // HINAPE_FLUID_DETAIL_SPH_KERNELS3_INL_H_
