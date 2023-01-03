#ifndef HINAPE_API_H
#define HINAPE_API_H

#include "renderer/app.h"

#include "geometry/sphere3.h"
#include "emitter/volume_particle_emitter3.h"

class Test { public: virtual void test_one_frame() = 0; };
using TestPtr = std::shared_ptr<Test>;

class TestEmitter : public Test
{
public:
	TestEmitter();
	void test_one_frame() final;

private:
	HinaPE::Fluid::ParticleEmitter3Ptr _emitter;
	HinaPE::Fluid::ParticleSystemData3Ptr _data;
	double time{0.0};
};

class PCISPHExample : public Kasumi::Api
{
public:
	void prepare() final;
	void step(float dt) final;
	void ui_sidebar() final;
	void key(int key, int scancode, int action, int mods) override;
	void sync();

	std::vector<TestPtr> _tests;
};

#endif //HINAPE_API_H
