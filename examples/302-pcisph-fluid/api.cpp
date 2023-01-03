#include "api.h"

#include "GLFW/glfw3.h"

TestEmitter::TestEmitter()
{
	auto sphere = HinaPE::Sphere3::builder()
			.withCenter({0, 0, 0})
			.withRadius(2)
			.makeShared();
	_emitter = HinaPE::Fluid::VolumeParticleEmitter3::builder()
			.withSurface(sphere)
			.withIsOneShot(false)
			.makeShared();

	_data = std::make_shared<HinaPE::Fluid::ParticleSystemData3>();
	_emitter->setTarget(_data);
}

void TestEmitter::test_one_frame()
{
	_emitter->update(time, 0.01f);
}

void PCISPHExample::prepare()
{
	_tests.push_back(std::make_shared<TestEmitter>());
}
void PCISPHExample::step(float dt) {}
void PCISPHExample::ui_sidebar() {}
void PCISPHExample::sync() {}
void PCISPHExample::key(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_N)
		for (auto const &test: _tests)
			test->test_one_frame();
}
