#ifndef HINAPE_SPH_KERNEL_H
#define HINAPE_SPH_KERNEL_H

#include "common.h"

namespace HinaPE
{
struct SPHKernel : public CopyDisable
{
public:
	virtual auto operator()(real distance) const -> real = 0;
	virtual auto first_derivative(real distance) const -> real = 0;
	virtual auto second_derivative(real distance) const -> real = 0;
	virtual auto gradient(const mVector3 &point) const -> mVector3 = 0;
	virtual auto gradient(real distance, const mVector3 &direction) const -> mVector3 = 0;
	explicit SPHKernel(real kernel_radius);

public:
	real h, h2, h3, h4, h5;
};

struct StdKernel : public SPHKernel
{
public:
	auto operator()(real distance) const -> real final;
	auto first_derivative(real distance) const -> real final;
	auto second_derivative(real distance) const -> real final;
	auto gradient(const mVector3 &point) const -> mVector3 final;
	auto gradient(real distance, const mVector3 &direction) const -> mVector3 final;
    auto cubic_kernel_derivative(real distance) const -> real ; // temp here
	explicit StdKernel(real kernelRadius);
};

struct SpikyKernel : public SPHKernel
{
public:
	auto operator()(real distance) const -> real final;
	auto first_derivative(real distance) const -> real final;
	auto second_derivative(real distance) const -> real final;
	auto gradient(const mVector3 &point) const -> mVector3 final;
	auto gradient(real distance, const mVector3 &direction) const -> mVector3 final;
	explicit SpikyKernel(real kernelRadius);
};

using SPHKernelPtr = std::shared_ptr<SPHKernel>;
using StdKernelPtr = std::shared_ptr<StdKernel>;
using SpikyKernelPtr = std::shared_ptr<SpikyKernel>;
} // namespace HinaPE

#endif //HINAPE_SPH_KERNEL_H
