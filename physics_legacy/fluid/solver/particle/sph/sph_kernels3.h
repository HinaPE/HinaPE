#ifndef HINAPE_FLUID_SPH_KERNELS3_H_
#define HINAPE_FLUID_SPH_KERNELS3_H_

#include "math/vector3.h"

#include "constants.h"

namespace HinaPE::Fluid
{

//!
//! \brief Standard 3-D SPH kernel function object.
//!
//! \see Müller, Matthias, David Charypar, and Markus Gross.
//!     "Particle-based fluid simulation for interactive applications."
//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
//!     animation. Eurographics Association, 2003.
//!
struct SphStdKernel3
{
    //! Kernel radius.
    double h;

    //! Square of the kernel radius.
    double h2;

    //! Cubic of the kernel radius.
    double h3;

    //! Fifth-power of the kernel radius.
    double h5;

    //! Constructs a kernel object with zero radius.
    SphStdKernel3();

    //! Constructs a kernel object with given radius.
    explicit SphStdKernel3(double kernelRadius);

    //! Copy constructor
    SphStdKernel3(const SphStdKernel3 &other);

    //! Returns kernel function value at given distance.
    auto operator()(double distance) const -> double;

    //! Returns the first derivative at given distance.
    auto firstDerivative(double distance) const -> double;

    //! Returns the gradient at a point.
    auto gradient(const Vector3D &point) const -> Vector3D;

    //! Returns the gradient at a point defined by distance and direction.
    auto gradient(double distance, const Vector3D &direction) const -> Vector3D;

    //! Returns the second derivative at given distance.
    auto secondDerivative(double distance) const -> double;
};

//!
//! \brief Spiky 3-D SPH kernel function object.
//!
//! \see Müller, Matthias, David Charypar, and Markus Gross.
//!     "Particle-based fluid simulation for interactive applications."
//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
//!     animation. Eurographics Association, 2003.
//!
struct SphSpikyKernel3
{
    //! Kernel radius.
    double h;

    //! Square of the kernel radius.
    double h2;

    //! Cubic of the kernel radius.
    double h3;

    //! Fourth-power of the kernel radius.
    double h4;

    //! Fifth-power of the kernel radius.
    double h5;

    //! Constructs a kernel object with zero radius.
    SphSpikyKernel3();

    //! Constructs a kernel object with given radius.
    explicit SphSpikyKernel3(double h_ /* kernelRadius */);

    //! Copy constructor
    SphSpikyKernel3(const SphSpikyKernel3 &other);

    //! Returns kernel function value at given distance.
    auto operator()(double distance) const -> double;

    //! Returns the first derivative at given distance.
    auto firstDerivative(double distance) const -> double;

    //! Returns the gradient at a point.
    auto gradient(const Vector3D &point) const -> Vector3D;

    //! Returns the gradient at a point defined by distance and direction.
    auto gradient(double distance, const Vector3D &direction) const -> Vector3D;

    //! Returns the second derivative at given distance.
    auto secondDerivative(double distance) const -> double;
};

}  // namespace HinaPE::Fluid

#include "sph_kernels3-inl.h"

#endif  // HINAPE_FLUID_SPH_KERNELS3_H_
