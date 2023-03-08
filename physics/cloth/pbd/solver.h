#ifndef HINAPE_CLOTH_SOLVER_H
#define HINAPE_CLOTH_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "../objects/cloth.h"
#include "constraints.h"

namespace HinaPE
{
class PBDClothSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
	void update(real dt);

public:
	struct Opt
	{
		real time_step = 0.02f;
		real gravity = -9.8f;
		real damping = 0.99f;

		bool distance_constraint = true;
		real distance_constraint_stiffness = 0.9f;
	} _opt;
	PBDClothSolver();

protected:
	void _prepare();
	void _external_force() const;
	void _time_integration() const;
	void _constraint_projection();
	void _update_state() const;
	void INSPECT() final;

public:
	std::shared_ptr<ClothObject> _data;
	std::vector<ConstraintPtr> _constraints;
	real _current_dt;
};

using PBDClothSolverPtr = std::shared_ptr<PBDClothSolver>;
} // namespace HinaPE

#endif //HINAPE_CLOTH_SOLVER_H
