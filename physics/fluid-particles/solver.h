#ifndef HINAPE_FLUID_PARTICLES_SOLVER_H
#define HINAPE_FLUID_PARTICLES_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"

namespace HinaPE
{
class SPHSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
	struct Data;
	std::shared_ptr<Data> _data = std::make_shared<Data>();
	BoxDomainPtr _domain;

	void update(real dt);

public:
	struct Opt
	{
		// common
		bool inited = false;
		real current_dt = 0.02; // don't alter this
		mVector3 gravity = mVector3(0, -9.8, 0);

		//sph
		real eos_exponent = 7;
		real negative_pressure_scale = 0.0;
		real viscosity_coefficient = 0.01;
		real pseudo_viscosity_coefficient = 10.0;
		real speed_of_sound = 100;
		real time_step_limit_scale = 0.4;
	} _opt;

protected:
	void INSPECT() final;
	void VALID_CHECK() const final;
};

// @formatter:off
struct SPHSolver::Data : public CopyDisable, public Kasumi::ObjectParticles3D
{
public:
	// particles
	std::vector<mVector3> 	_positions;
	std::vector<mVector3> 	_velocities;
	std::vector<mVector3> 	_forces;
	std::vector<real> 		_densities;
	std::vector<real> 		_pressures;

protected:
	void _update_poses() final;
};
// @formatter:on

using SPHSolverPtr = std::shared_ptr<SPHSolver>;
using SPHSolverDataPtr = std::shared_ptr<SPHSolver::Data>;
} // namespace HinaPE

#endif //HINAPE_FLUID_PARTICLES_SOLVER_H
