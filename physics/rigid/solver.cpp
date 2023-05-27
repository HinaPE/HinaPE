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

void HinaPE::RigidSolver::add(const Kasumi::ObjectMesh3DPtr &object, RigidType type)
{
	auto *rb = world->createRigidBody(toR(object->POSE));
	rb->setType(toR(type));
	_objects.emplace_back(object, rb);
	reactphysics3d::Collider * collider = nullptr;
	if (is<Kasumi::SphereObject>(object.get()))
	{
		collider = rb->addCollider(physicsCommon.createSphereShape(as<Kasumi::SphereObject>(object.get())->_radius), reactphysics3d::Transform::identity());
	} else if (is<Kasumi::CubeObject>(object.get())) {
		collider = rb->addCollider(physicsCommon.createBoxShape(toR(as<Kasumi::CubeObject>(object.get())->_extent)), reactphysics3d::Transform::identity());
	}
	collider->getMaterial().setBounciness(0.9);
	collider->getMaterial().setFrictionCoefficient(0.01);
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

void HinaPE::RigidSolver::apply_force_and_torque(int index, const mVector3 &force_world, const mVector3 &torque_world)
{
    reactphysics3d::RigidBody *rb = world->getRigidBody(index);

    /*const auto& transform = rb->getTransform();
    const auto& position = transform.getPosition();
    const auto& rotation = transform.getOrientation();
    reactphysics3d::Vector3 localForce = rotation.getInverse() * toR(force_world);

    const auto& invOrientation = rotation.getInverse().getMatrix();
    reactphysics3d::Vector3 localTorque = invOrientation * toR(torque_world);*/

    rb->applyWorldForceAtCenterOfMass(toR(force_world));
    rb->applyWorldTorque(toR(torque_world));


}

void HinaPE::RigidSolver::clear_force_and_torque(int index) {
    reactphysics3d::RigidBody *rb = world->getRigidBody(index);
    auto zero_force = mVector3 (0);
    rb->applyWorldForceAtCenterOfMass(toR(zero_force));
    rb->applyWorldTorque(toR(zero_force));
}
