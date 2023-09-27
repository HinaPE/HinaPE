#include "sph_kernel.h"

HinaPE::SPHKernel::SPHKernel(real kernel_radius) : h(kernel_radius), h2(h * h), h3(h2 * h), h4(h2 * h2), h5(h2 * h3) {}
HinaPE::StdKernel::StdKernel(real kernelRadius) : SPHKernel(kernelRadius) {}
HinaPE::SpikyKernel::SpikyKernel(real kernelRadius) : SPHKernel(kernelRadius) {}
auto HinaPE::StdKernel::operator()(real distance) const -> real
{
	if (distance * distance >= h2)
		return 0;

	real x = 1.0 - distance * distance / h2;
	return 315.0 / (64.0 * HinaPE::Constant::PI * h3) * x * x * x;
}
auto HinaPE::StdKernel::first_derivative(real distance) const -> real
{
	if (distance > h)
		return 0;

	real x = 1.0 - distance * distance / h2;
	return -945.0 / (32.0 * HinaPE::Constant::PI * h5) * distance * x * x;
}
auto HinaPE::StdKernel::second_derivative(real distance) const -> real
{
	if (distance * distance > h2)
		return 0;

	real x = distance * distance / h2;
	return 945.0 / (32.0 * HinaPE::Constant::PI * h5) * (1 - x) * (5 * x - 1);
}
auto HinaPE::StdKernel::gradient(const mVector3 &point) const -> mVector3
{
	real dist = point.length();
	if (dist > 0)
		return gradient(dist, point / dist);
	return mVector3::Zero();
}
auto HinaPE::StdKernel::gradient(real distance, const mVector3 &direction) const -> mVector3
{
	return -first_derivative(distance) * direction;
}

auto HinaPE::StdKernel::cubic_kernel_derivative(real distance) const -> real {
    if (distance > h)
        return 0;
    real x = 1.0 - distance / h;
    return -30.0 / (HinaPE::Constant::PI * h4) * x * x;
}

auto HinaPE::SpikyKernel::operator()(real distance) const -> real
{
	if (distance > h)
		return 0;

	real x = 1.0 - distance / h;
	return 15.0 / (HinaPE::Constant::PI * h3) * x * x * x;
}
auto HinaPE::SpikyKernel::first_derivative(real distance) const -> real
{
	if (distance > h)
		return 0;

	real x = 1.0 - distance / h;
	return -45.0 / (HinaPE::Constant::PI * h4) * x * x;
}
auto HinaPE::SpikyKernel::second_derivative(real distance) const -> real
{
	if (distance > h)
		return 0;

	real x = 1.0 - distance / h;
	return 90.0 / (HinaPE::Constant::PI * h5) * x;
}
auto HinaPE::SpikyKernel::gradient(const mVector3 &point) const -> mVector3
{
	real dist = point.length();
	if (dist > 0)
		return gradient(dist, point / dist);
	return mVector3::Zero();
}
auto HinaPE::SpikyKernel::gradient(real distance, const mVector3 &direction) const -> mVector3
{
	return -first_derivative(distance) * direction;
}

auto HinaPE::CohesionKernel::operator()(real distance) const -> real {
    real res = 0.0;
    if(distance * distance < h2)
    {
        if (distance > 0.5 * h)
            res = static_cast<real>(32.0) / (HinaPE::Constant::PI * pow(h, static_cast<real>(9.0))) * pow(h - distance, static_cast<real>(3.0)) * pow(distance,static_cast<real>(3.0));
        else
            res = static_cast<real>(32.0) / (HinaPE::Constant::PI * pow(h, static_cast<real>(9.0))) * static_cast<real>(2.0) * pow(h - distance, static_cast<real>(3.0)) * pow(distance,static_cast<real>(3.0)) - pow(h, static_cast<real>(6.0)) / static_cast<real>(64.0);
    }
    return res;
}

HinaPE::CohesionKernel::CohesionKernel(real kernelRadius) : SPHKernel(kernelRadius) { }

auto HinaPE::CohesionKernel::first_derivative(real distance) const -> real {
    return 0;
}

auto HinaPE::CohesionKernel::second_derivative(real distance) const -> real {
    return 0;
}

auto HinaPE::CohesionKernel::gradient(const mVector3 &point) const -> mVector3 {
    return mVector3();
}

auto HinaPE::CohesionKernel::gradient(real distance, const mVector3 &direction) const -> mVector3 {
    return mVector3();
}


