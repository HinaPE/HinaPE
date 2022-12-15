#include "rigid_solver.h"

// make sure there is no reactphysics3d content in the header file.
#include "reactphysics3d/reactphysics3d.h"

class RigidApi
{
public:
	static reactphysics3d::PhysicsCommon common;
	static reactphysics3d::PhysicsWorld *world;
	static std::map<unsigned int, reactphysics3d::RigidBody *> _rbs;
};
reactphysics3d::PhysicsCommon RigidApi::common;
reactphysics3d::PhysicsWorld *RigidApi::world = RigidApi::common.createPhysicsWorld();
std::map<unsigned int, reactphysics3d::RigidBody *> RigidApi::_rbs;

auto HinaPE::Rigid::RigidSolver::add_rigidbody(const RigidOpt &rb_opt, ShapeOpt *shape_opt, const mVector3 &pos, const mVector3 &rot, const mVector3 &scale) -> unsigned int
{
	reactphysics3d::Vector3 position(pos.x, pos.y, pos.z);
	auto q = mQuaternion(rot.x, rot.y, rot.z);
	reactphysics3d::Quaternion orientation = {q.x, q.y, q.z, q.w};
	reactphysics3d::Transform transform(position, orientation);

	auto rb = RigidApi::world->createRigidBody(transform);
	rb->setLinearVelocity({rb_opt.vel.x, rb_opt.vel.y, rb_opt.vel.z});
	rb->setAngularVelocity({rb_opt.ang_vel.x, rb_opt.ang_vel.y, rb_opt.ang_vel.z});
	rb->setMass(rb_opt.mass);
	rb->setLinearDamping(rb_opt.friction);
	switch (rb_opt.type)
	{
		case Type::STATIC:
			rb->setType(reactphysics3d::BodyType::STATIC);
			break;
		case Type::KINEMATIC:
			rb->setType(reactphysics3d::BodyType::KINEMATIC);
			break;
		case Type::DYNAMIC:
			rb->setType(reactphysics3d::BodyType::DYNAMIC);
			break;
		default:
			break;
	}

	if (shape_opt->type == ShapeType::SPHERE)
	{
		auto sphere_opt = dynamic_cast<SphereOpt*>(shape_opt);
		reactphysics3d::SphereShape *sphereShape = RigidApi::common.createSphereShape(sphere_opt->radius);
		rb->addCollider(sphereShape, reactphysics3d::Transform::identity());
	} else if (shape_opt->type == ShapeType::BOX)
	{
		auto box_opt = dynamic_cast<BoxOpt *>(shape_opt);
		reactphysics3d::BoxShape *boxShape = RigidApi::common.createBoxShape({box_opt->x, box_opt->y, box_opt->z});
		rb->addCollider(boxShape, reactphysics3d::Transform::identity());
	}
	rb->updateMassPropertiesFromColliders();

	static unsigned int static_id = 0;
	auto id = static_id++;
	RigidApi::_rbs[id] = rb;
	return id;
}
auto HinaPE::Rigid::RigidSolver::get_position(unsigned int id) -> mVector3
{
	auto pos = RigidApi::_rbs[id]->getTransform().getPosition();
	return {pos.x, pos.y, pos.z};
}
auto HinaPE::Rigid::RigidSolver::get_rotation(unsigned int id) -> mVector3
{
	auto quat = RigidApi::_rbs[id]->getTransform().getOrientation();
	return mQuaternion(quat.w, quat.x, quat.y, quat.z).euler();
}
void HinaPE::Rigid::RigidSolver::update(float dt)
{
	RigidApi::world->update(dt);
}
