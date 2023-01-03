#ifndef HINAPE_TEST_EMITTER_H
#define HINAPE_API_H

#include "renderer/app.h"
#include "renderer/scene/scene.h"

#include "geometry/bounding_box3.h"
#include "geometry/plane3.h"
#include "geometry/box3.h"
#include "geometry/cylinder3.h"
#include "geometry/rigid_body_collider3.h"
#include "geometry/sphere3.h"
#include "geometry/implicit_surface_set3.h"
#include "logging.h"
#include "array/array_utils-inl.h"
#include "solver/particle/sph/sph_solver3.h"
#include "solver/particle/sph/sph_solver3.h"
#include "solver/particle/pcisph/pci_sph_solver3.h"
#include "emitter/volume_particle_emitter3.h"

class SPHFluidExample : public Kasumi::Api
{
public:
	void prepare() override;
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
	Kasumi::SceneObjectPtr _fluid_obj;
	Kasumi::SceneObjectPtr _bbox_obj;
};

#endif //HINAPE_TEST_EMITTER_H
