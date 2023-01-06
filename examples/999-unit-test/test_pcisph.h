#ifndef HINAPE_TEST_PCISPH_H
#define HINAPE_TEST_PCISPH_H

#include "renderer/app.h"

#include "emitter/volume_particle_emitter3.h"
#include "solver/particle/pcisph/pci_sph_solver3.h"
#include "geometry/triangle_mesh3.h"
#include "geometry/box3.h"
#include "geometry/rigid_body_collider3.h"

#include <thread>

class TestPciSPH : public Kasumi::Api
{
public:
	void prepare() final;
	void key(int key, int scancode, int action, int mods) final;
	void ui_sidebar() final;

protected:
	void sync() const;
	HinaPE::Frame _frame = HinaPE::Frame(0, 1 / 60.0);

private:
	HinaPE::Fluid::PciSphSolver3Ptr _solver;
	HinaPE::Fluid::VolumeParticleEmitter3Ptr _emitter;

	// scene part
	Kasumi::SceneObjectPtr _fluid_obj;

	std::thread _physics_thread;
	bool _advance_frame = false;
	bool _is_complete = true;
	bool _should_close = false;
};

#endif //HINAPE_TEST_PCISPH_H
