#ifndef HINAPE_FLUID_PARTICLES_SOLVER_H
#define HINAPE_FLUID_PARTICLES_SOLVER_H

#include "domain/box_domain.h"

namespace HinaPE
{
class SPHSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
	struct Data;
	BoxDomainPtr _domain;

public:
	void update(real dt);

protected:
	void INSPECT() final;
	void VALID_CHECK() const final;
};

struct SPHSolver::Data : public CopyDisable, public Kasumi::ObjectParticles3D
{
	// particles
	std::vector<mVector3> _positions;
	std::vector<mVector3> _velocities;
	std::vector<mVector3> _forces;
	std::vector<real> _densities;
	std::vector<real> _pressures;
};

using SPHSolverPtr = std::shared_ptr<SPHSolver>;
using SPHSolverDataPtr = std::shared_ptr<SPHSolver::Data>;
} // namespace HinaPE

#endif //HINAPE_FLUID_PARTICLES_SOLVER_H
