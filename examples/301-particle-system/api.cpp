#include "api.h"
void ParticleSystem::prepare()
{
	// init render object
	auto particles_model = std::make_shared<Kasumi::Model>("sphere", Color::RED);
	particles_model->instancing();
	auto id = _scene->add_object(particles_model);
	_obj = _scene->get_object(id);

	// init physics object
	_solver = std::make_shared<HinaPE::Fluid::ParticleSystemSolver3>();

	sync();
}
void ParticleSystem::step(float dt) {}
void ParticleSystem::ui_sidebar() {}
void ParticleSystem::key(int key, int scancode, int action, int mods) {}
void ParticleSystem::sync()
{
	auto const &positions = _solver->particle_positions();

	auto model = _obj->get_model();
	std::vector<Kasumi::Pose> poses;
	for (auto const &pos: positions)
	{
		Kasumi::Pose pose;
		pose.position = {pos.x, pos.y, pos.z};
		pose.scale = {0.01, 0.01, 0.01};
		poses.push_back(pose);
	}
	model->clear_instances();
	if (!poses.empty())
		model->add_instances(poses);
}
