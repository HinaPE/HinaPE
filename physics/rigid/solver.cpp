#include "solver.h"

// @formatter:off
static auto toR(const mVector3 &v) -> reactphysics3d::Vector3 { return reactphysics3d::Vector3{v.x(), v.y(), v.z()}; }
static auto toR(const mQuaternion &q) -> reactphysics3d::Quaternion { return reactphysics3d::Quaternion{q.x(), q.y(), q.z(), q.w()}; }
static auto toR(const Kasumi::Pose &p) -> reactphysics3d::Transform { return reactphysics3d::Transform{toR(p.position), toR(mQuaternion(p.euler))}; }
static auto toR(const HinaPE::RigidType &r) -> reactphysics3d::BodyType { switch (r) { case HinaPE::RigidType::Static: return reactphysics3d::BodyType::STATIC; case HinaPE::RigidType::Dynamic: return reactphysics3d::BodyType::DYNAMIC; case HinaPE::RigidType::Kinematic: return reactphysics3d::BodyType::KINEMATIC; default: return reactphysics3d::BodyType::DYNAMIC; } }
static auto toM(const reactphysics3d::Vector3 &v) -> mVector3 { return mVector3{v.x, v.y, v.z}; }
static auto toM(const reactphysics3d::Quaternion &q) -> mQuaternion { return mQuaternion{q.w, q.x, q.y, q.z}; }
static auto toM(const reactphysics3d::Transform &t) -> Kasumi::Pose { return Kasumi::Pose{toM(t.getPosition()), toM(t.getOrientation()).euler(), {1, 1, 1}}; }
// @formatter:on

HinaPE::RigidSolver::RigidSolver() : world(physicsCommon.createPhysicsWorld()) {}

auto HinaPE::RigidSolver::add(const Kasumi::ObjectMesh3DPtr &object, RigidType type) -> reactphysics3d::RigidBody*
{
	auto *rb = world->createRigidBody(toR(object->POSE));
    rb->setMass(5);
	rb->setType(toR(type));
	_objects.emplace_back(object, rb);
	reactphysics3d::Collider * collider = nullptr;
	if (dynamic_cast<Kasumi::SphereObject *>(object.get()))
	{
		collider = rb->addCollider(physicsCommon.createSphereShape(dynamic_cast<Kasumi::SphereObject *>(object.get())->_radius), reactphysics3d::Transform::identity());
	} else if (dynamic_cast<Kasumi::CubeObject *>(object.get())) {
		collider = rb->addCollider(physicsCommon.createBoxShape(toR(dynamic_cast<Kasumi::CubeObject *>(object.get())->_extent)), reactphysics3d::Transform::identity());
	}
	collider->getMaterial().setBounciness(0.9);
	collider->getMaterial().setFrictionCoefficient(0.01);
	return rb;
}

void HinaPE::RigidSolver::update(real dt)
{
	world->update(dt);
	for (auto &[object, rb]: _objects)
	{
		auto pose = toM(rb->getTransform());
		object->POSE.position = pose.position;
		object->POSE.euler = pose.euler;
	}
}

void HinaPE::RigidSolver::apply_force_and_torque(reactphysics3d::RigidBody* rigid, const mVector3 &force_world, const mVector3 &torque_world)
{
    rigid->applyWorldForceAtCenterOfMass(toR(force_world));
    rigid->applyWorldTorque(toR(torque_world));
}

void HinaPE::RigidSolver::clear_force_and_torque(reactphysics3d::RigidBody* rigid) {
    auto zero_force = mVector3 (0);
    rigid->applyWorldForceAtCenterOfMass(toR(zero_force));
    rigid->applyWorldTorque(toR(zero_force));
}
