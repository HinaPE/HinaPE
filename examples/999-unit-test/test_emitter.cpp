#include "test_emitter.h"

#include "GLFW/glfw3.h"

void TestEmitter::prepare()
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

	// scene obj
	auto model = std::make_shared<Kasumi::Model>("sphere", Color::RED);
	model->instancing();
	_fluid_obj = _scene->get_object(_scene->add_object(model));
}

void TestEmitter::key(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_N)
	{
		one_frame();
		sync();
	}
}

void TestEmitter::one_frame()
{
	_emitter->update(_time, 0.01f);
	_time += 0.01f;
}

void TestEmitter::sync() const
{
	auto const &positions = _data->positions();
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
