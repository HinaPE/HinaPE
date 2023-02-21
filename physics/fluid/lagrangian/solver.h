#ifndef HINAPE_SOLVER_H
#define HINAPE_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "renderer3D/renderer3D.h"

#include "common.h"
#include "emitter/point_particle_emitter.h"
#include "domain/fluid_domain.h"
#include "neighbor_search/point_simple_list_searcher.h"

namespace HinaPE
{
class SPHSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::Renderer3D
{
public:
	void init() final;
	void step(real dt) final;

public:
	struct Opt
	{
		// common
		bool inited = false;
		real current_dt = 0.02; // don't alter this
		mVector3 gravity = mVector3(0, -9.8, 0);

		//sph
		real eos_exponent = 7;
		real negative_pressure_scale = 0.0;
		real viscosity_coefficient = 0.01;
		real pseudo_viscosity_coefficient = 10.0;
		real speed_of_sound = 100;
		real time_step_limit_scale = 0.4;
	} _opt;
	struct Data;
	struct Kernel;
	void INSPECT() final;
	SPHSolver();

protected:
	void _accumulate_force() const;
	void _time_integration() const;
	void _resolve_collision() const;

private:
	void _resize_buffer();
	void _build_neighbor() const;
	void _update_collider() const;
	void _update_emitter() const;
	void _update_density() const;
	void _update_pressure() const;

private:
	std::shared_ptr<Data> _data;
	std::shared_ptr<Kernel> _kernel;
	std::shared_ptr<ParticleEmitter3> _emitter;
	std::shared_ptr<FluidDomain3> _domain;

	// temp buffer
	std::vector<mVector3> _new_positions;
	std::vector<mVector3> _new_velocities;
};
using SPHSolverPtr = std::shared_ptr<SPHSolver>;


struct SPHSolver::Data : public CopyDisable, public Kasumi::ParticlesObject
{
public:
	struct Opt
	{
		// common
		real mass = 1e-3;
		real radius = 1e-3;
		real max_search_radius = 1e-3;

		// sph
		real target_density = 1000; // water density
		real target_spacing = 0.1;
		real kernel_radius_over_target_spacing = 1.8;
		real kernel_radius = kernel_radius_over_target_spacing * target_spacing;
	} _opt;
	Data() { NAME = "SPHSolver::Data"; }
	void build_neighbor();
	void update();
	void resize();
	auto size() const -> size_t;

public:
	// particles
	std::vector<mVector3> _positions;
	std::vector<mVector3> _velocities;
	std::vector<mVector3> _forces;
	std::vector<real> _densities;
	std::vector<real> _pressures;

	// neighbor searcher
	PointNeighborSearcher3Ptr _neighbor_searcher;
	std::vector<std::vector<size_t>> _neighbor_lists;
};
using SPHSolverDataPtr = std::shared_ptr<SPHSolver::Data>;


struct SPHSolver::Kernel : public CopyDisable
{
public:
	virtual auto operator()(real distance) const -> real = 0;
	virtual auto first_derivative(real distance) const -> real = 0;
	virtual auto second_derivative(real distance) const -> real = 0;
	virtual auto gradient(const mVector3 &point) const -> mVector3 = 0;
	virtual auto gradient(real distance, const mVector3 &direction) const -> mVector3 = 0;
};

struct StdKernel : public SPHSolver::Kernel
{
public:
	auto operator()(real distance) const -> real final;
	auto first_derivative(real distance) const -> real final;
	auto second_derivative(real distance) const -> real final;
	auto gradient(const mVector3 &point) const -> mVector3 final;
	auto gradient(real distance, const mVector3 &direction) const -> mVector3 final;

public:
	struct Opt
	{
		real kernel_radius = 0.18;
	} _opt;
	void _rebuild_();

public:
	real h, h2, h3, h5;
};
using SPHSolverKernelPtr = std::shared_ptr<SPHSolver::Kernel>;
} // namespace HinaPE

#endif //HINAPE_SOLVER_H
