#ifndef HINAPE_FMS_KERNEL_H
#define HINAPE_FMS_KERNEL_H

#include "../../common.h"

namespace HinaPE
{
class PhysicsSystem;

class FastMassSpringKernel
{
public:
    void simulate(PhysicsSystem &sys, float dt);

    struct Opt
    {
        float dt = 1 / 30.f;
    };
    Opt opt;

private:
};
}
#endif //HINAPE_FMS_KERNEL_H
