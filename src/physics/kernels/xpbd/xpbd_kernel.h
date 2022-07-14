#ifndef HINAPE_XPBD_KERNEL_H
#define HINAPE_XPBD_KERNEL_H

namespace HinaPE
{
class PhysicsSystem;

class XPBDKernel
{
public:
    void simulate(PhysicsSystem &sys, float dt);
};

}


#endif //HINAPE_XPBD_KERNEL_H
