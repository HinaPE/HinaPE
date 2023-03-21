#ifndef HINAPE_PCISPH_SOLVER_H
#define HINAPE_PCISPH_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"
#include "domain/sphere_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"
#include "neighbor_search/compact_nsearch.h"

namespace HinaPE
{
class PCISPHSolver final : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
    /// const?
	void init();
	void update(real dt) const;

public:
	struct Opt
	{
		bool inited = false;
		real current_dt = 0.02;
		mVector3 gravity = mVector3(0, -9.8, 0);
		real restitution = 0.3;
	} _opt;
	struct Data;
	std::shared_ptr<Data> 		_data;
	Geom::RigidBodyCollider3Ptr _domain;
	ParticleEmitter3Ptr 		_emitter;

protected:
	void _emit_particles() const;
	void _accumulate_non_pressure_force() const;
    void _accumulate_pressure_force() const;
	void _time_integration() const;
	void _resolve_collision() const;

	void INSPECT() final;
	void VALID_CHECK() const final;
};

struct PCISPHSolver::Data : public CopyDisable, public Kasumi::ObjectParticles3D
{
	// particles
	std::vector<mVector3> 	_positions;
	std::vector<mVector3> 	_velocities;
	std::vector<mVector3> 	_forces;
	std::vector<real> 		_densities;
	std::vector<real> 		_pressures;

    std::vector<mVector3> 	_temp_positions;
    std::vector<mVector3> 	_temp_velocities;
    std::vector<mVector3> 	_pressures_forces;
    std::vector<real> 	    _density_errors;
    std::vector<real> 	    _predict_densities;

	// params
	real _mass 				= 1e-3; // should be recalculated  to fit water density
	real _radius 			= 0.02;

	real target_density 	= 1000; // water density
	real target_spacing 	= _radius;
	real kernel_radius_over_target_spacing = 1.8;
	real kernel_radius 		= target_spacing * kernel_radius_over_target_spacing;

	// pcisph
	real eos_exponent 					= 7;
	real negative_pressure_scale 		= 0.0;
	real viscosity_coefficient 			= 0.01;
	real pseudo_viscosity_coefficient 	= 10.0;
	real speed_of_sound 				= 100;


	SPHKernelPtr kernel = std::make_shared<StdKernel>(kernel_radius);
	PointNeighborSearch3Ptr _neighbor_search = std::make_shared<PointHashGridSearch3>(_radius);
	std::vector<std::vector<unsigned int>> _neighbor_lists;

	Data();
	friend class PCISPHSolver;
	void _update_neighbor();
	void _update_density();
    void _update_predict_density();
	void _update_pressure();
	void _update_mass();
    void _update_pressure_force();
	void INSPECT() final;

	bool _mass_inited = false;
};
using PCISPHSolverPtr = std::shared_ptr<PCISPHSolver>;
using PCISPHSolverDataPtr = std::shared_ptr<PCISPHSolver::Data>;
} // namespace HinaPE

#endif //HINAPE_PCISPH_SOLVER_H
