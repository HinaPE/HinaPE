#ifndef HINAPE_FMS_KERNEL_H
#define HINAPE_FMS_KERNEL_H

#include "../../common.h"

namespace HinaPE
{
class PhysicsSystem;

class FastMassSpringKernel final
{
public:
    auto simulate(PhysicsSystem &sys, float dt) -> void;

    struct Opt
    {
        Eigen::Vector3f gravity;
        Eigen::Vector3f wind_direction;
        Eigen::Vector3f wind_speed;
        float fixed_dt = 1 / 30.f;
    };
    Opt opt;

private:
    auto simulate_for_each(Eigen::Map<Eigen::MatrixXf> &pos, Eigen::Map<Eigen::MatrixXf> &vel, Eigen::Map<Eigen::MatrixXf> &frc, float dt) -> void;
};
}
#endif //HINAPE_FMS_KERNEL_H
