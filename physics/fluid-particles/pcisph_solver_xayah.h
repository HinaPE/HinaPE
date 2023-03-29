#ifndef HINAPE_PCISPH_SOLVER_XAYAH_H
#define HINAPE_PCISPH_SOLVER_XAYAH_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"
#include "domain/sphere_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"

namespace HinaPE
{
class PCISPHSolverXayah : public Kasumi::INSPECTOR
{
protected:
	void _accumulate_force() const;
	void _time_integration() const;
	void _resolve_collision() const;
	void _update_neighbor() const;
	void _update_density() const;

public:
	void init();
	void update(real dt) const;
	void reset();

	struct Data;
	std::shared_ptr<Data> _data;
	std::shared_ptr<BoxDomain> _domain;
	std::shared_ptr<VolumeParticleEmitter3> _emitter;

	struct
	{
		real current_dt = 0.005;
		mVector3 gravity = mVector3(0, -9.8, 0);
		real restitution 	= 0.3;

		// fluid param
		real radius 		= 0.029;
		real target_density = 1000; // dont alter after inited
		real relative_kernel_radius = 2.7; // this is important!
		real kernel_radius 	= relative_kernel_radius * radius;

		// SPH options
		real eos 				= 7;
		real nps 				= 0.0; // negative pressure scale
		real viscosity 			= 0.01;
		real pseudo_viscosity 	= 10.0;
		real vorticity 			= 0.00001;
		real speed_of_sound 	= 100;

		// PCISPH options
		real max_density_error_ratio = 0.01;
		size_t max_iterations = 10;
	} Opt;

private:
	void _init_fluid_particles() const;
	auto _compute_delta() const -> real;
	void INSPECT() override;
};

struct PCISPHSolverXayah::Data : public Kasumi::ObjectParticles3D
{
	struct // fluid particles
	{
		std::vector<mVector3> 	positions;
		std::vector<mVector3> 	predicted_position;
		std::vector<mVector3> 	velocities;
		std::vector<mVector3> 	forces;
		std::vector<real> 		densities;
		std::vector<real> 		pressures;
		real					mass; // should be recalculated  to fit target density

		// predictive & corrective
		std::vector<mVector3> 	temp_positions;
		std::vector<mVector3> 	temp_velocities;
		std::vector<mVector3> 	temp_pressure_forces;
		std::vector<real> 		temp_density_errors;

		// for debug
		std::vector<mVector3> 	last_positions;
	} Fluid;

	std::vector<std::vector<unsigned int>> 	NeighborList;

	explicit Data();
	void add_fluid(const std::vector<mVector3>& positions, const std::vector<mVector3>& velocities);
	auto fluid_size() const -> size_t;
	void reset();


	// ==================== Debug Area ====================

	std::vector<mVector3> color_map;
	std::vector<std::vector<std::string>> debug_info;

	// ==================== Debug Area ====================
};
} // namespace HinaPE

#endif //HINAPE_PCISPH_SOLVER_XAYAH_H
