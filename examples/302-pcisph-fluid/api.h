#ifndef HINAPE_TEST_EMITTER_H
#define HINAPE_API_H

#include "renderer/app.h"

#include "geometry/sphere3.h"
#include "emitter/volume_particle_emitter3.h"

class TestEmitter
{
public:
	TestEmitter();
	void test_one_frame() const;

	HinaPE::Fluid::ParticleEmitter3Ptr _emitter;
	HinaPE::Fluid::ParticleSystemData3Ptr _data;
	double _time{0.0};
};
using TestEmitterPtr = std::shared_ptr<TestEmitter>;

class PCISPHExample : public Kasumi::Api
{
public:
	void prepare() final;
	void ui_sidebar() final;
	void key(int key, int scancode, int action, int mods) final;
	void sync() const;

	TestEmitterPtr _test_emitter;

	// scene part
	Kasumi::SceneObjectPtr _fluid_obj;
};

#endif //HINAPE_TEST_EMITTER_H
