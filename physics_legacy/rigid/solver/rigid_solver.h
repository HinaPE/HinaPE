#ifndef HINAPE_RIGID_SOLVER_H
#define HINAPE_RIGID_SOLVER_H

#include "cloth/math_api.h"

#include "shape.h"

#include <map>
#include <memory>

namespace HinaPE::Rigid
{
class RigidSolver
{
public:
	enum class Type { STATIC, KINEMATIC, DYNAMIC };
	struct RigidOpt
	{
		mVector3 vel = {0, 0, 0};
		mVector3 ang_vel = {0, 0, 0};
		float mass = 1;
		float friction = 0.5;
		float restitution = 0.5;
		Type type = Type::DYNAMIC;
	};
	static auto add_rigidbody(const RigidOpt& rb_opt, ShapeOpt *shape_opt, const mVector3 &pos = {0, 0, 0}, const mVector3 &rot = {0, 0, 0}, const mVector3 &scale = {1, 1, 1}) -> unsigned int;
	static auto get_position(unsigned int id) -> mVector3;
	static auto get_rotation(unsigned int id) -> mVector3;
	static void update(float dt);
};
using RigidSolverPtr = std::shared_ptr<RigidSolver>;
}

#endif //HINAPE_RIGID_SOLVER_H
