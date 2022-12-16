#ifndef HINAPE_SPH_SOLVER3_H
#define HINAPE_SPH_SOLVER3_H

#include "_solver/particle/particle_system_solver3.h"

namespace HinaPE::Fluid
{
class SPHSolver3 : public ParticleSystemSolver3
{
public:
	struct Opt : public ParticleSystemSolver3::Opt
	{
	} _opt;
protected:
	void on_begin_particle_system_update(real dt) final;
	void on_end_particle_system_update(real dt) final;
};
using SPHSolver3Ptr = std::shared_ptr<SPHSolver3>;
}

#endif //HINAPE_SPH_SOLVER3_H
