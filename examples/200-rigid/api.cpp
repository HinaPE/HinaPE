#include "api.h"
#include "imgui.h"

#include <random>
static std::mt19937 mt;
static std::random_device rnd;
static auto random(float min, float max) -> float { return min + (max - min) * (mt() % 10000 / 10000.f); }

void RigidExample::prepare()
{
	mt.seed(rnd());

	{
		mVector3 pos = {0, 10, 0};

		// init physics world
		HinaPE::Rigid::RigidSolver::RigidOpt _rb_opt;
		_rb_opt.type = HinaPE::Rigid::RigidSolver::Type::DYNAMIC;
		_rb_opt.vel = {random(0, 1), random(0, 1), random(0, 1)};
		HinaPE::Rigid::SphereOpt _sphere_opt;
		_sphere_opt.radius = 1;
		unsigned id = HinaPE::Rigid::RigidSolver::add_rigidbody(_rb_opt, &_sphere_opt, pos);

		// init rendering world
		mVector3 color = {random(0, 1), random(0, 1), random(0, 1)};
		auto sphere_id = _scene->add_object(std::make_shared<Kasumi::Model>("sphere", color));
		auto sphere = _scene->get_object(sphere_id);

		_objs.emplace_back(sphere, id);
	}

	{
		mVector3 pos = {0, -7, 0};
		mVector3 size = {15, 1, 15};
		// init physics world
		HinaPE::Rigid::RigidSolver::RigidOpt _rb_opt;
		_rb_opt.type = HinaPE::Rigid::RigidSolver::Type::STATIC;
		HinaPE::Rigid::BoxOpt _box_opt;
		_box_opt.x = size.x;
		_box_opt.y = size.y;
		_box_opt.z = size.z;
		unsigned id = HinaPE::Rigid::RigidSolver::add_rigidbody(_rb_opt, &_box_opt, pos);

		// init rendering world
		auto floor_id = _scene->add_object(std::make_shared<Kasumi::Model>("cube", Color::GRAY));
		auto floor = _scene->get_object(floor_id);
		floor->position() = pos;
		floor->scale() = size;

		_objs.emplace_back(floor, id);
	}

	sync();
}
void RigidExample::step(float dt)
{
	HinaPE::Rigid::RigidSolver::update(dt);
	sync();
}
void RigidExample::ui_sidebar()
{
	if (ImGui::Button("Add a sphere"))
	{
		mVector3 pos = {0, 10, 0};

		// init physics world
		HinaPE::Rigid::RigidSolver::RigidOpt _rb_opt;
		_rb_opt.type = HinaPE::Rigid::RigidSolver::Type::DYNAMIC;
		_rb_opt.vel = {random(0, 1), random(0, 1), random(0, 1)};
		HinaPE::Rigid::SphereOpt _sphere_opt;
		_sphere_opt.radius = 1;
		unsigned id = HinaPE::Rigid::RigidSolver::add_rigidbody(_rb_opt, &_sphere_opt, pos);

		// init rendering world
		mVector3 color = {random(0, 1), random(0, 1), random(0, 1)};
		auto sphere_id = _scene->add_object(std::make_shared<Kasumi::Model>("sphere", color));
		auto sphere = _scene->get_object(sphere_id);

		_objs.emplace_back(sphere, id);
	}

	if (ImGui::Button("Add a cube"))
	{
		mVector3 pos = {0, 10, 0};
		mVector3 size = {1, 1, 1};

		// init physics world
		HinaPE::Rigid::RigidSolver::RigidOpt _rb_opt;
		_rb_opt.type = HinaPE::Rigid::RigidSolver::Type::DYNAMIC;
		_rb_opt.vel = {random(0, 1), random(0, 1), random(0, 1)};
		HinaPE::Rigid::BoxOpt _box_opt;
		_box_opt.x = size.x;
		_box_opt.y = size.y;
		_box_opt.z = size.z;
		unsigned id = HinaPE::Rigid::RigidSolver::add_rigidbody(_rb_opt, &_box_opt, pos);

		// init rendering world
		mVector3 color = {random(0, 1), random(0, 1), random(0, 1)};
		auto cube_id = _scene->add_object(std::make_shared<Kasumi::Model>("cube", color));
		auto cube = _scene->get_object(cube_id);
		cube->position() = pos;
		cube->scale() = size;

		_objs.emplace_back(cube, id);
	}
}
void RigidExample::sync()
{
	for (auto &pair: _objs)
	{
		pair.first->_pose.position = HinaPE::Rigid::RigidSolver::get_position(pair.second);
		pair.first->_pose.euler = HinaPE::Rigid::RigidSolver::get_rotation(pair.second);
	}
}
