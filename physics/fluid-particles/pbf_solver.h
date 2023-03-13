#ifndef HINAPE_PBF_SOLVER_H
#define HINAPE_PBF_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"
#include "neighbor_search/compact_nsearch.h"

namespace HinaPE
{
class PBFSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
	void init();
	void update(real dt) const;

public:
	struct Opt
	{
		bool inited = false;
		real current_dt = 0.02; // don't alter this
		mVector3 gravity = mVector3(0, -9.8, 0);
		real restitution = 0.3;
	} _opt;
	struct Data;
	struct DensityConstraints;
	std::shared_ptr<Data> _data;
	std::shared_ptr<DensityConstraints> _density_constraints;
	BoxDomainPtr _domain;
	ParticleEmitter3Ptr _emitter;

protected:
	void _emit_particles() const;
	void _accumulate_force() const;
	void _time_integration() const;
	void _resolve_collision() const;

protected:
	void INSPECT() final;
	void VALID_CHECK() const final;
};

// @formatter:off
struct PBFSolver::Data : public CopyDisable, public Kasumi::ObjectParticles3D
{
public:
	// particles
	std::vector<mVector3> 	_positions;
	std::vector<mVector3> 	_velocities;
	std::vector<mVector3> 	_forces;
	std::vector<real> 		_densities;
	std::vector<real> 		_pressures;

	// temporary buffers
	std::vector<real> 		_lambdas;
	std::vector<mVector3> 	_delta_p;

	// params
	real _mass 				= 1e-3; // should be recalculated  to fit water density
	real _radius 			= 0.02;

	real target_density 	= 1000; // water density
	real target_spacing 	= _radius;
	real kernel_radius_over_target_spacing = 1.8;
	real kernel_radius 		= target_spacing * kernel_radius_over_target_spacing;


	SPHKernelPtr poly6_kernel = std::make_shared<StdKernel>(kernel_radius);
	SPHKernelPtr spiky_kernel = std::make_shared<SpikyKernel>(kernel_radius);
	PointNeighborSearch3Ptr _neighbor_search = std::make_shared<PointHashGridSearch3>(_radius);
	std::vector<std::vector<unsigned int>> _neighbor_lists;

	Data();
	friend class PBFSolver;
	void _update_neighbor();
	void _update_density();
	void _update_mass();
	void INSPECT() final;

	bool _mass_inited = false;
};
// @formatter:on

struct PBFSolver::DensityConstraints
{
	explicit DensityConstraints(std::shared_ptr<PBFSolver::Data> data) : _data(std::move(data)) {}
	void solve() const;

	std::shared_ptr<PBFSolver::Data> _data;
};

using PBFSolverPtr = std::shared_ptr<PBFSolver>;
using PBFSolverDataPtr = std::shared_ptr<PBFSolver::Data>;
using PBFDensityConstraintsPtr = std::shared_ptr<PBFSolver::DensityConstraints>;
} // namespace HinaPE

#endif //HINAPE_PBF_SOLVER_H
