#ifndef HINAPE_PCISPH_TEMP_H
#define HINAPE_PCISPH_TEMP_H

#include "domain/box_domain.h"
#include "domain/sphere_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"

namespace HinaPE
{
    class PCISPHSolverTEMP: public Kasumi::INSPECTOR
    {
    protected:
        void _update_fluid_neighbor() const;
        void _update_boundary_neighbor() const;
        void _update_density() const;
        void _resolve_collision() const;
        void _accumulate_non_pressure_force() const;
        void _initialize_pressure_and_pressure_force() const;
        void _prediction_correction_step();
        void _predict_velocity_and_position() const;
        void _predict_density() const;
        void _update_pressure();
        void _accumulate_pressure_force();
        void _correct_velocity_and_position() const;
        void _update_boundary_volume() const;
        void _compute_boundary_forces() const;
        void _transfer_force_to_center() const;
        void _compute_rigid_forces_and_torque() const;

        void _compute_E() const;
    public:
        void init();
        void update(real dt);
        void reset();

        struct Data;
        std::shared_ptr<Data> 					_data;
        std::shared_ptr<BoxDomain> 				_domain;
        std::shared_ptr<VolumeParticleEmitter3> _emitter;
        std::shared_ptr<Kasumi::SphereObject> 	_sphere;
        std::shared_ptr<Kasumi::CubeObject> 	_cube;

        struct Opt
        {
            real current_dt = 0.002;
            mVector3 gravity = mVector3(0, -9.8, 0);
            real restitution = 0.3;

            // fluid param
            real radius 			= 0.029;
            real target_density 	= 1000; // water density
            real target_spacing 	= radius;
            real relative_kernel_radius = 1.7;
            real kernel_radius 		= target_spacing * relative_kernel_radius;

            // SPH options
            real eos 				= 7;
            real nps 				= 0.0; // negative pressure scale
            real viscosity 			= 0.01;
            real pseudo_viscosity 	= 10.0;
            real vorticity 			= 0.00001;
            real speed_of_sound 	= 100;

            // PCISPH options
            real min_loop = 3;
            real max_loop = 50;
            real max_density_error_ratio = 0.01;
            bool density_error_too_large = false;

            bool use_akinci2012_collision = false;
        }_opt;

    private:
        void _init_fluid_particles() const;
        void _init_boundary_particles() const;
        void _init_collider() const;
        void _init_boundary_density() const;
        auto _compute_delta() const -> real;
        void INSPECT() override;
    };

    struct PCISPHSolverTEMP::Data : public Kasumi::ObjectParticles3D
    {
        struct // fluid particles
        {
            std::vector<mVector3> 	positions;
            std::vector<mVector3> 	predicted_positions;

            std::vector<mVector3> 	velocities;
            std::vector<mVector3> 	predicted_velocities;

            std::vector<mVector3> 	non_pressure_forces;
            std::vector<mVector3> 	pressure_forces;

            std::vector<real> 		densities;
            std::vector<real> 	    predicted_densities;
            std::vector<real> 	    density_errors;

            std::vector<real> 		pressures;
            real					mass;

            // for debug
            std::vector<mVector3> 	last_positions;
        }Fluid;

        struct // boundary particles
        {
            std::vector<mVector3> 	positions;
            std::vector<mVector3> 	positions_origin;
            std::vector<real>		mass;
            std::vector<real>		volume;
            std::vector<real>		density;

            std::vector<mVector3> 	pressure_forces;
            std::vector<mVector3> 	friction_forces;
            std::vector<mVector3> 	forces;

            std::vector<bool> 	    IsActive;

            std::vector<const Kasumi::Pose*> 		poses;
            std::vector<std::pair<size_t, size_t>> 	boundary_sizes;
        } Boundary;

        struct{
            std::vector<mVector3> force;
            std::vector<mVector3> torque;
        } ForceAndTorque;

        mVector3 F_total_b;

        std::vector<std::vector<unsigned int>> 	FluidNeighborList;
        std::vector<std::vector<unsigned int>> 	BoundaryNeighborList;

        explicit Data();
        void add_fluid(const std::vector<mVector3>& positions, const std::vector<mVector3>& velocities);
        void add_boundary(const std::vector<mVector3>& positions, const Kasumi::Pose* pose);
        void update_boundary();
        auto fluid_size() const -> size_t;
        auto boundary_size() const -> size_t;
        void reset();

        // ==================== Debug Area ====================

        std::vector<mVector3> color_map;
        std::vector<std::vector<std::string>> debug_info;

        // ==================== Debug Area ====================
    };
}

#endif //HINAPE_PCISPH_TEMP_H
