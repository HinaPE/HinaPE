#include "fms_kernel.h"
#include "../../physics_system.h"

auto HinaPE::FastMassSpringKernel::simulate(HinaPE::PhysicsSystem &sys, float dt) -> void
{
    auto &os = sys.physics_objects;
    for (auto &pair: os)
    {
        auto &o = pair.second;
        if (o->get_type() == Deformable)
        {
            auto &pos = o->get_vertices();
            auto &vel = o->get_velocities();
            auto &mass = o->get_masses();
            Eigen::Map<Eigen::MatrixXf> M_positions(pos.data()->data, static_cast<int>(pos.size()), 3);
            Eigen::Map<Eigen::MatrixXf> M_velocities(vel.data()->data, static_cast<int>(vel.size()), 3);
            Eigen::Map<Eigen::MatrixXf> M_masses(vel.data()->data, static_cast<int>(vel.size()), 3);

            simulate_for_each(M_positions, M_velocities, M_positions, dt);
        }
    }
}

auto HinaPE::FastMassSpringKernel::simulate_for_each(Eigen::Map<Eigen::MatrixXf> &pos, Eigen::Map<Eigen::MatrixXf> &vel, Eigen::Map<Eigen::MatrixXf> &frc, float dt) -> void
{

}

auto HinaPE::FastMassSpringKernel::local_step() -> void
{

}

auto HinaPE::FastMassSpringKernel::global_step() -> void
{

}
