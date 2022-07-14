#ifndef HINAPE_PBD_KERNEL_H
#define HINAPE_PBD_KERNEL_H

namespace HinaPE
{
class PhysicsSystem;

class PBDKernel
{
public:
    static void simulate(PhysicsSystem& sys, float dt);
};

}


#endif //HINAPE_PBD_KERNEL_H
