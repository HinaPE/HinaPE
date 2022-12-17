#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/app.h"
#include "_solver/particle/particle_system_solver3.h"

class ParticleSystem : public Kasumi::Api
{
public:
	void prepare() final;
	void step(float dt) final;
	void ui_sidebar() final;
	void key(int key, int scancode, int action, int mods) final;
	void sync();

private:
	HinaPE::Fluid::ParticleSystemSolver3Ptr _solver;
	Kasumi::SceneObjectPtr _fluid_obj;
	Kasumi::SceneObjectPtr _bbox_obj;

private:
	mBBox domain;
};

#endif //HINAPE_API_H
