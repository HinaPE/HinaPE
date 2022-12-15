#ifndef HINAPE_SPH_SOLVER3_H
#define HINAPE_SPH_SOLVER3_H

#include "_solver/particle/particle_system_solver3.h"

namespace HinaPE::Fluid
{
class SPHSolver3 : public ParticleSystemSolver3
{
};
using SPHSolver3Ptr = std::shared_ptr<SPHSolver3>;
}

#endif //HINAPE_SPH_SOLVER3_H
