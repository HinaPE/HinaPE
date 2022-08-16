#ifndef HINAPE_XPBD_KERNEL_H
#define HINAPE_XPBD_KERNEL_H

namespace HinaPE
{
class PhysicsSystem;

class XPBDKernel final
{
public:
    void simulate(float dt);
};

}

#endif //HINAPE_XPBD_KERNEL_H
