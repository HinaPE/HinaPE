#ifndef HINAPE_FMS_KERNEL_H
#define HINAPE_FMS_KERNEL_H

#include "../../common.h"

namespace HinaPE
{
class PhysicsSystem;

class FastMassSpringKernel
{
public:
    auto simulate(PhysicsSystem &sys, float dt)->void;

    struct Opt
    {
        float dt = 1 / 30.f;
    };
    Opt opt;

private:
    auto simulate_for_each(Eigen::Map<Eigen::MatrixXf> &pos, Eigen::Map<Eigen::MatrixXf> &vel, Eigen::Map<Eigen::MatrixXf> &acc, float dt)->void;
};
}
#endif //HINAPE_FMS_KERNEL_H
