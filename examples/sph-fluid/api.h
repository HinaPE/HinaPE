#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/api.h"
#include "renderer/scene/scene.h"

#include "common/geometry/bounding_box3.h"
#include "common/geometry/plane3.h"
#include "common/geometry/box3.h"
#include "common/geometry/cylinder3.h"
#include "common/geometry/rigid_body_collider3.h"
#include "common/geometry/sphere3.h"
#include "common/geometry/implicit_surface_set3.h"
#include "common/logging.h"
#include "common/array/array_utils-inl.h"
#include "fluid/solver/particle/sph/sph_solver3.h"
#include "fluid/solver/particle/sph/sph_solver3.h"
#include "fluid/solver/particle/pcisph/pci_sph_solver3.h"
#include "fluid/emitter/volume_particle_emitter3.h"

class Api : public Kasumi::Api
{
public:
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
	auto phase1() -> bool;
	auto phase2() -> bool;

private:
	// physics part
	HinaPE::Fluid::SphSolver3Ptr _fluid_solver;
	HinaPE::Fluid::ParticleEmitter3Ptr _fluid_emitter;
	HinaPE::RigidBodyCollider3Ptr _fluid_collider;

	// scene part
	Kasumi::ModelPtr _bounding_model;
	Kasumi::ModelPtr _particle_model;
	Kasumi::SceneObjectPtr _bounding_model_scene;
	Kasumi::SceneObjectPtr _particle_model_scene;

	bool _inited = false;
};

#endif //HINAPE_API_H
