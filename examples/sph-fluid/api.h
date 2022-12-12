#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "renderer/scene/scene.h"
#include "fluid/solver/particle/sph/sph_solver3.h"

class Api : public Kasumi::Api
{
public:
	void step(float dt) final;
	void ui_sidebar() final;
	void sync();

private:
	enum FluidType { SPH };
	struct FluidOpt
	{
		float target_density = 1000.f;
		float target_spacing = 0.05f;

		FluidType type = SPH;
		// SPH Field
		float pseudo_viscosity_coefficient = 0.f;
	} _opt;

private:
	auto phase1() -> bool;
	auto phase2() -> bool;

private:
	HinaPE::Fluid::SphSolver3Ptr _fluid_solver;
	Kasumi::ModelPtr _fluid_bounding_model;
	Kasumi::ModelPtr _fluid_particle_model;
};

#endif //HINAPE_API_H
