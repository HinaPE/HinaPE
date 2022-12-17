#include "api.h"

#include "geometry/sphere3.h"
#include "geometry/plane3.h"

void ParticleSystem::prepare()
{
	// init render object
	auto particles_model = std::make_shared<Kasumi::Model>("sphere", Color::RED);
	particles_model->instancing();
	auto id = _scene->add_object(particles_model);
	_obj = _scene->get_object(id);

	// init physics object
//	auto plane = HinaPE::Plane3::builder()
//			.withNormal({0, 1, 0})
//			.withPoint({0, 0.25 * _opt.bounding_box_size, 0})
//			.makeShared();
//	auto sphere = HinaPE::Sphere3::builder()
//			.withCenter({_bounding_model_scene->position().x, _bounding_model_scene->position().y, _bounding_model_scene->position().z})
//			.withRadius(0.15 * _opt.bounding_box_size)
//			.makeShared();
//	auto surfaceSet = HinaPE::ImplicitSurfaceSet3::builder()
//			.withExplicitSurfaces({plane, sphere})
//			.makeShared();
//	mBBox domain;
//	_solver = std::make_shared<HinaPE::Fluid::ParticleSystemSolver3>(surface_set, domain);

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
