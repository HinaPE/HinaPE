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

    // 将世界坐标系下的力和力矩转换为刚体本地坐标系下的向量
    // 将力和力矩分别从世界坐标系下的向量转换为齐次坐标系下的向量（添加一个分量为 1 的齐次坐标）。
    // 使用刚体的逆旋转矩阵将齐次坐标系下的向量从世界坐标系转换为刚体本地坐标系。
    // 将转换后的向量去掉齐次分量（将第四个分量除以前三个分量）。
    //reactphysics3d::RigidBody *rb = world->getRigidBody(0);
    //mVector3 worldForce =
    //mVector3 localForce = rb->getTransform().getInverse().GetRotationMatrix() * mVector4(worldForce, 1.0f);
    // 怎么拿出来我不会！！！
    // 啊啊啊啊啊啊啊啊啊啊啊啊GetRotationMatrix怎么拿
    // mVector4好像也不能这么……
    // 调用刚体的 applyForceAtCenterOfMass 和 applyTorque 函数，将转换后的力和力矩应用到刚体上
}
