#include "fms_kernel.h"
#include "../../physics_system.h"

#include "Eigen/Eigen"

void HinaPE::FastMassSpringKernel::simulate(HinaPE::PhysicsSystem &sys, float dt)
{
    auto &os = sys.physics_objects;
    for (auto &pair: os)
    {
        auto &o = pair.second;
        if (o->get_type() == Deformable)
        {
            auto &pos = o->pos();

            auto func = [&]
            {
                Eigen::Map<Eigen::MatrixXf> M(pos.data()->data, static_cast<int>(pos.size()), 3);
            };

            check_time(func);

//            std::cout << M.rows() << std::endl;
//            std::cout << M.cols() << std::endl;
        }
    }
}
