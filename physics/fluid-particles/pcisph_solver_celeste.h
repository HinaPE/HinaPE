#ifndef HINAPE_PCISPH_SOLVER_CELESTE_H
#define HINAPE_PCISPH_SOLVER_CELESTE_H

#include "domain/box_domain.h"
#include "domain/sphere_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"

namespace HinaPE
{
    class PCISPHSolverCELESTE final : public Kasumi::INSPECTOR
    {
    protected:
        void _update_neighbor() const;
        void _update_density() const;
        void _accumulate_non_pressure_force() const;
        void _initialize_pressure_and_pressure_force() const;
        void _prediction_correction_step();
        void _predict_velocity_and_position() const;
        void _predict_density() const;
        void _update_pressure();
        void _accumulate_pressure_force() const;
        void _correct_velocity_and_position() const;
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
            real current_dt = 0.01;
            mVector3 gravity = mVector3(0, -9.8, 0);
            real restitution = 0.3;

            real min_loop = 3;
            real max_loop = 50;

            // fluid param
            real radius 			= 0.02;
            real target_density 	= 1000; // water density
            real target_spacing 	= radius;
            real relative_kernel_radius = 1.8;
            real kernel_radius 		= target_spacing * relative_kernel_radius;
            real viscosity 		= 0.1;

            real max_density_error_ratio = 0.01;
            bool density_error_too_large = true;
            // options
            bool use_akinci2012_collision = true;
        }_opt;

    private:
        void _init_fluid_particles() const;
        void _init_boundary_particles() const;
        void _init_collider() const;
        void _reset_debug_info() const;
        void INSPECT() override;
    };

    struct PCISPHSolverCELESTE::Data : public Kasumi::ObjectParticles3D
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
            std::vector<real> 		delta_pressure;

            std::vector<real>       delta;
            real					mass;
        }Fluid;

        struct // boundary particles
        {
            std::vector<mVector3> 	positions;
            real					mass; // should be recalculated  to fit target density
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
        std::vector<std::vector<mVector3>> delta_p_debug;

        // ==================== Debug Area ====================
    };
}
#endif //HINAPE_PCISPH_SOLVER_CELESTE_H
