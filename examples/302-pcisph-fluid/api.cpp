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
#include <iostream>
void TestEmitter::test_one_frame() const
{
	_emitter->update(_time, 0.01f);
	std::cout << _data->positions().size() << std::endl;
}

void PCISPHExample::prepare()
{
	_test_emitter = std::make_shared<TestEmitter>();

	// scene obj
	auto model = std::make_shared<Kasumi::Model>("sphere", Color::RED);
	model->instancing();
	_fluid_obj = _scene->get_object(_scene->add_object(model));
}
void PCISPHExample::ui_sidebar() {}
void PCISPHExample::sync() const
{
	auto const &positions = _test_emitter->_data->positions();
	std::vector<Kasumi::Pose> poses;
	for (auto const &pos: positions)
	{
		Kasumi::Pose pose;
		pose.position = {pos.x, pos.y, pos.z};
		pose.scale = {0.01, 0.01, 0.01};
		poses.push_back(pose);
	}
	_fluid_obj->get_model()->clear_instances();
	if (!poses.empty())
		_fluid_obj->get_model()->add_instances(poses);
}
void PCISPHExample::key(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_N)
	{
		_test_emitter->test_one_frame();
		sync();
	}
}
