#ifndef HINAPE_RIGID_SOLVER_H
#define HINAPE_RIGID_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "renderer3D/renderer3D.h"
#include "reactphysics3d/reactphysics3d.h"

namespace HinaPE
{
enum class RigidType
{
	Static,
	Dynamic,
	Kinematic
};
class RigidSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
	void add(const Kasumi::ObjectMesh3DPtr &object, RigidType type = RigidType::Dynamic);
	void update(real dt);

public:
	RigidSolver();

protected:
	void INSPECT() final;

private:
	std::vector<std::pair<Kasumi::ObjectMesh3DPtr, reactphysics3d::RigidBody *>> _objects;
	reactphysics3d::PhysicsCommon physicsCommon;
	reactphysics3d::PhysicsWorld *world;
};
using RigidSolverPtr = std::shared_ptr<RigidSolver>;
} // namespace HinaPE

#endif //HINAPE_RIGID_SOLVER_H
