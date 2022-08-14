#ifndef HINAPE_SPH_KERNEL_H
#define HINAPE_SPH_KERNEL_H

#include "../../common.h"

namespace HinaPE
{
class PhysicsSystem;

class SPHKernel
{
public:
    auto simulate(PhysicsSystem &sys, float dt) -> void;

private:

};
}

#endif //HINAPE_SPH_KERNEL_H
