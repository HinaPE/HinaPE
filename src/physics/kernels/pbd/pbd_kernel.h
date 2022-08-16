#ifndef HINAPE_PBD_KERNEL_H
#define HINAPE_PBD_KERNEL_H

#include "../../common.h"

namespace HinaPE
{
class PhysicsSystem;

class PBDKernel final
{
public:
    void simulate(float dt);

    struct Opt
    {
        float dt = 1 / 30.f;
    };
    Opt opt;

private:
    void init(const Opt &opt);
    void prediction(class PhysicsObject &);
    void collision_detection();
    void project_constraint();
    void update_states();

private: // time integration
    using real = float;
    void semi_implicit_euler(real h, real mass, Vec3 &position, Vec3 &velocity, const Vec3 &acceleration);
};

}

#endif //HINAPE_PBD_KERN