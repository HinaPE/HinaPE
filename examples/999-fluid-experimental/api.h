#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "_solver/particle/particle_system_solver3.h"

#include "renderer/app.h"
#include "renderer/scene/scene.h"

class FluidExperimental : public Kasumi::Api
{
public:
	void prepare() final;
	void step(float dt) final;
	void ui_sidebar() final;
	void sync();

private:
	struct SPHFluidOpt
	{
		float bounding_box_size = 1.f;
		float target_density = 1000.f;
		float target_spacing = 0.05f;

		// SPH Field
		float pseudo_viscosity_coefficient = 0.f;
	} _opt;

private:
	HinaPE::Fluid::ParticleSystemSolver3Ptr _solver;

	// scene part
	Kasumi::ModelPtr _bounding_model;
	Kasumi::ModelPtr _particle_model;
	Kasumi::SceneObjectPtr _bounding_model_scene;
	Kasumi::SceneObjectPtr _particle_model_scene;

	bool _inited = false;
};

#endif //HINAPE_API_H
