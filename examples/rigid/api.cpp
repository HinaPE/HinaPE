#include "api.h"
#include "imgui.h"

using namespace reactphysics3d;

#include <random>
static std::mt19937 mt;
static std::random_device rnd;
static auto random(float min, float max) -> float { return min + (max - min) * (mt() % 10000 / 10000.f); }

void Api::prepare()
{
	mt.seed(rnd());

	// init physics world
	world = common.createPhysicsWorld();

	Vector3 position(0, 10, 0);
	Quaternion orientation = Quaternion::identity();
	Transform transform(position, orientation);
	auto sphere_rb = world->createRigidBody(transform);

	SphereShape *sphereShape = common.createSphereShape(1);
	sphere_rb->addCollider(sphereShape, Transform::identity());
	sphere_rb->updateMassPropertiesFromColliders();

	Vector3 p(0.0, -7.0, 0.0);
	Quaternion o = Quaternion::identity();
	Transform t(p, o);
	auto floor_rb = world->createRigidBody(t);
	BoxShape *floorShape = common.createBoxShape({15, 1, 15});
	floor_rb->addCollider(floorShape, Transform::identity());
	floor_rb->setType(BodyType::STATIC);

	// init rendering world
	mVector3 color = {random(0, 1), random(0, 1), random(0, 1)};
	auto sphere_id = _scene->add_object(std::make_shared<Kasumi::Model>("sphere", color));
	auto sphere = _scene->get_object(sphere_id);
	sphere->get_model()->_opt.render_wireframe = true;
	auto floor_id = _scene->add_object(std::make_shared<Kasumi::Model>("cube", Color::GRAY));
	auto floor = _scene->get_object(floor_id);

	floor->position() = {0, -7, 0};
	floor->scale() = {15, 1, 15};

	_objs.emplace_back(sphere, sphere_rb);
	_objs.emplace_back(floor, floor_rb);

	sync();
}
void Api::step(float dt)
{
	world->update(dt);
	sync();
}
void Api::ui_sidebar()
{
	if (ImGui::Button("Add a sphere"))
	{
		Vector3 position(0, 10, 0);
		Quaternion orientation = Quaternion::identity();
		Transform transform(position, orientation);
		auto sphere_rb = world->createRigidBody(transform);
		SphereShape *sphereShape = common.createSphereShape(1);
		sphere_rb->addCollider(sphereShape, Transform::identity());
		sphere_rb->updateMassPropertiesFromColliders();
		sphere_rb->setLinearVelocity({random(0, 1), random(0, 1), random(0, 1)});

		mVector3 color = {random(0, 1), random(0, 1), random(0, 1)};
		auto sphere_id = _scene->add_object(std::make_shared<Kasumi::Model>("sphere", color));
		auto sphere = _scene->get_object(sphere_id);
		sphere->get_model()->_opt.render_wireframe = true;

		_objs.emplace_back(sphere, sphere_rb);
	}

	if (ImGui::Button("Add a cube"))
	{
		Vector3 position(0, 10, 0);
		Quaternion orientation = Quaternion::identity();
		Transform transform(position, orientation);
		auto cube_rb = world->createRigidBody(transform);
		BoxShape *boxShape = common.createBoxShape({1, 1, 1});
		cube_rb->addCollider(boxShape, Transform::identity());
		cube_rb->updateMassPropertiesFromColliders();
		cube_rb->setLinearVelocity({random(0, 1), random(0, 1), random(0, 1)});

		mVector3 color = {random(0, 1), random(0, 1), random(0, 1)};
		auto cube_id = _scene->add_object(std::make_shared<Kasumi::Model>("cube", color));
		auto cube = _scene->get_object(cube_id);
		cube->get_model()->_opt.render_wireframe = true;

		_objs.emplace_back(cube, cube_rb);
	}
}
void Api::sync()
{
	for (auto &pair: _objs)
	{
		const auto &trans = pair.second->getTransform();
		const auto &pos = trans.getPosition();
		const auto &quat = trans.getOrientation();
		pair.first->_pose.position = {pos.x, pos.y, pos.z};
		pair.first->_pose.euler = mQuaternion(quat.w, quat.x, quat.y, quat.z).euler();
	}
}
