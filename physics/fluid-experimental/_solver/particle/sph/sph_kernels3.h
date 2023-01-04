#ifndef HINAPE_SPH_KERNELS3_H
#define HINAPE_SPH_KERNELS3_H

#include "math_api.h"
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
	explicit SphStdKernel3(real r) : h(r), h2(r * r), h3(h2 * r), h5(h3 * h2) {}
	inline auto operator()(real d) const -> real { return d * d >= h2 ? static_cast<real>(0) : static_cast<real>(315.0 / (64.0 * 3.141592653589 * h3) * std::pow(1.0 - d * d / h2, 3)); }
	inline auto first_derivative(real d) const -> real { return d * d >= h2 ? static_cast<real>(0) : static_cast<real>(-945.0 / (32.0 * 3.141592653589 * h5) * d * std::pow(1.0 - d * d / h2, 2)); }
	inline auto gradient(const mVector3 &p) const -> mVector3 { return p.length() > 0 ? p * first_derivative(p.length()) : mVector3(); }
	inline auto gradient(real d, const mVector3 &dir) const -> mVector3 { return dir * first_derivative(d); }
	real h, h2, h3, h5;
};
//!
//! \brief Spiky 3-D SPH kernel function object.
//!
//! \see Müller, Matthias, David Charypar, and Markus Gross.
//!     "Particle-based fluid simulation for interactive applications."
//!     Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
//!     animation. Eurographics Association, 2003.
//!
struct SphSpikeyKernel3
{
	real h, h2, h3, h4, h5;
};
}

#endif //HINAPE_SPH_KERNELS3_H
