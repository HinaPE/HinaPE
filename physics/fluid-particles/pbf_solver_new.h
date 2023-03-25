#ifndef HINAPE_PBF_SOLVER_NEW_H
#define HINAPE_PBF_SOLVER_NEW_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"
#include "domain/sphere_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"

// @formatter:off
namespace HinaPE
{
class PBFSolverNew : public Kasumi::INSPECTOR
{
protected:
	void _apply_force_and_predict_position() const;
	void _update_neighbor() const;
	void _update_density() const;
	void _solve_density_constraints() const;
	void _update_positions_and_velocities() const;

public:
	void init();
	void update(real dt) const;
	void reset();

	struct Data;
	std::shared_ptr<Data> 					_data;
	std::shared_ptr<BoxDomain> 				_domain;
	std::shared_ptr<VolumeParticleEmitter3> _emitter;
	std::shared_ptr<Kasumi::SphereObject> 	_sphere;
	std::shared_ptr<Kasumi::CubeObject> 	_cube;

	struct Opt
	{
		real current_dt = 0.005;
		mVector3 gravity = mVector3(0, -9.8, 0);
		real restitution = 0.3; // particle collision bouncing ratio (only for simple collision)
		int constraint_solver_iterations = 5;

		// fluid param
		real radius 		= 0.029;
		real target_density = 1000; // dont alter after inited
		real relative_kernel_radius = 1.7; // this is important!
		real kernel_radius 	= relative_kernel_radius * radius;
		real viscosity 		= 0.1;
		real vorticity 		= 0.00001;

		// options
		bool use_akinci2012_collision = true;
		bool enable_surface_tension = true;
		bool enable_vorticity = true;
		bool enable_viscosity = true;
	} _opt;

private:
	void _init_fluid_particles() const;
	void _init_boundary_particles() const;
	void _init_collider() const;
	void _reset_debug_info() const;
	void INSPECT() override;
};

struct PBFSolverNew::Data : public Kasumi::ObjectParticles3D
{
	struct // fluid particles
	{
		std::vector<mVector3> 	positions;
		std::vector<mVector3> 	predicted_position;
		std::vector<mVector3> 	velocities;
		std::vector<mVector3> 	forces;
		std::vector<real> 		densities;
		std::vector<real> 		lambdas;
		std::vector<mVector3> 	delta_p;
		real					mass; // should be recalculated  to fit target density
	} Fluid;
	struct // boundary particles
	{
		std::vector<mVector3> 	positions;
		std::vector<real>		mass; // should be recalculated  to fit target density
	} Boundary;

	std::vector<std::vector<unsigned int>> 	NeighborList;

	explicit Data();
	void add_fluid(const std::vector<mVector3>& positions, const std::vector<mVector3>& velocities);
	void add_boundary(const std::vector<mVector3>& positions);
	auto fluid_size() const -> size_t;
	auto boundary_size() const -> size_t;
	void reset();

	// ==================== Debug Area ====================

	std::vector<mVector3> color_map;
	std::vector<std::vector<std::string>> debug_info;

	std::vector<std::vector<unsigned int>> 	neighbor_list_debug;
	std::vector<std::vector<mVector3>> p_debug;
	std::vector<std::vector<real>> lambdas_debug;
	std::vector<std::vector<mVector3>> delta_p_debug;

	// ==================== Debug Area ====================
};
} // namespace HinaPE

#endif //HINAPE_PBF_SOLVER_NEW_H
