#include "api.h"
#include "imgui.h"

using namespace reactphysics3d;

void Api::prepare()
{
	mt.seed(rnd());            // シード指定

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
	BoxShape *floorShape = common.createBoxShape({10, 1, 10});
	floor_rb->addCollider(floorShape, Transform::identity());
	floor_rb->setType(BodyType::STATIC);

	// init rendering world
	mVector3 color = {mt() % 10000 / 10000.f, mt() % 10000 / 10000.f, mt() % 10000 / 10000.f};
	auto sphere_id = _scene->add_object(std::make_shared<Kasumi::Model>("sphere", color));
	auto sphere = _scene->get_object(sphere_id);
	auto floor_id = _scene->add_object(std::make_shared<Kasumi::Model>("cube", Color::GRAY));
	auto floor = _scene->get_object(floor_id);

	floor->position() = {0, -7, 0};
	floor->scale() = {10, 1, 10};

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

		mVector3 color = {mt() % 10000 / 10000.f, mt() % 10000 / 10000.f, mt() % 10000 / 10000.f};
		auto sphere_id = _scene->add_object(std::make_shared<Kasumi::Model>("sphere", color));
		auto sphere = _scene->get_object(sphere_id);

		_objs.emplace_back(sphere, sphere_rb);
	}
}
void Api::sync()
{
	for (auto &pair: _objs)
	{
		const auto &trans = pair.second->getTransform();
		const auto &pos = trans.getPosition();
		pair.first->_pose.position = {pos.x, pos.y, pos.z};
	}
}
