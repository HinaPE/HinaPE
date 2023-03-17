#include "solver.h"

//static auto toM(const b2Vec2 &v) -> mVector3 { return mVector3{v.x, v.y, v.z}; }
//static auto toM(const b2Rot &q) -> mQuaternion { return mQuaternion{q.w, q.x, q.y, q.z}; }4
//static auto toM(const b2Transform &t) -> Kasumi::Pose { return Kasumi::Pose{toM(t.p), toM(t.q), {1, 1, 1}}; }

HinaPE::Rigid2DSolver::Rigid2DSolver() : _world(b2Vec2(0, -9.8)) {}

void HinaPE::Rigid2DSolver::add(const Kasumi::Object2DPtr &object, HinaPE::RigidType type)
{
}

void HinaPE::Rigid2DSolver::update(real dt)
{
	_world.Step(static_cast<float>(dt), 6, 2);
//	for (auto &[object, rb]: _objects) { object->POSE = toM(rb->GetTransform()); }
}
