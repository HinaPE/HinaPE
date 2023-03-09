#ifndef HINAPE_SPH_SOLVER_H
#define HINAPE_SPH_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"
#include "neighbor_search/compact_nsearch.h"

// @formatter:off
namespace HinaPE
{
class SPHSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
	void update(real dt) const;

public:
	struct Opt
	{
		// common
		bool inited 						= false;
		real current_dt 					= 0.02; // don't alter this
		mVector3 gravity 					= mVector3(0, -9.8, 0);
		real restitution 					= 0.3;
	} _opt;
	struct Data;
	std::shared_ptr<Data> 	_data;
	BoxDomainPtr 			_domain;
	ParticleEmitter3Ptr 	_emitter;

protected:
	void _emit_particles() const;
	void _accumulate_force() const;
	void _time_integration() const;
	void _resolve_collision() const;

	void INSPECT() final;
	void VALID_CHECK() const final;
};


struct SPHSolver::Data : public CopyDisable, public Kasumi::ObjectParticles3D
{
public:
	// particles
	std::vector<mVector3> 	_positions;
	std::vector<mVector3> 	_velocities;
	std::vector<mVector3> 	_forces;
	std::vector<real> 		_densities;
	std::vector<real> 		_pressures;

	// params
	real _mass 				= 1e-3;
	real _radius 			= 1e-3;
	real max_search_radius 	= 1e-3;

	// sph
	real target_density 	= 1000; // water density
	real target_spacing 	= 0.1;
	real kernel_radius_over_target_spacing = 5.8;
	real kernel_radius 		= kernel_radius_over_target_spacing * target_spacing;

	real eos_exponent 					= 7;
	real negative_pressure_scale 		= 0.0;
	real viscosity_coefficient 			= 0.01;
	real pseudo_viscosity_coefficient 	= 10.0;
	real speed_of_sound 				= 100;


	PointNeighborSearch3Ptr _neighbor_search;

protected:
	friend class SPHSolver;
	void _update_poses() final;
	void _update_neighbor();
	void _update_density();
	void _update_pressure();
	void INSPECT() final;

	std::vector<std::vector<size_t>> _neighbor_lists;
};
// @formatter:on

using SPHSolverPtr = std::shared_ptr<SPHSolver>;
using SPHSolverDataPtr = std::shared_ptr<SPHSolver::Data>;
} // namespace HinaPE

#endif //HINAPE_SPH_SOLVER_H
