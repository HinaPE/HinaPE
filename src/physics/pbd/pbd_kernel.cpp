#include "pbd_kernel.h"
#include "../physics_system.h"

void HinaPE::PBDKernel::simulate(HinaPE::PhysicsSystem &sys, float dt)
{
    auto &os = sys.objects();

    for (auto &pair: os)
    {
        auto &o = pair.second;
        o->doing();
    }
}
