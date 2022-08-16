#include "fms_kernel.h"
#include "../../physics_system.h"

HinaPE::FastMassSpringKernel::FastMassSpringKernel(HinaPE::PhysicsSystem &sys) : physics_system(sys)
{
    auto &os = physics_system.physics_objects;
    for (auto &pair: os)
    {
        if (pair.second->is<DeformableBase<CLOTH>>())
        {
            auto id = pair.first;
            auto &o = pair.second->get_object<DeformableBase<CLOTH>>();

            auto &verts = o.vertices();
            Eigen::Map<Eigen::MatrixXf> M_vertices(verts.data()->data, static_cast<int>(verts.size()), 3);
            auto vertices_num = M_vertices.rows();

            TripletList LTriplets, JTriplets;
            SparseMatrix L;
            L.resize(3 * vertices_num, 3 * vertices_num);

            auto &vel = o.velocities();
            auto &mass = o.masses();
            Eigen::Map<Eigen::MatrixXf> M_velocities(vel.data()->data, static_cast<int>(vel.size()), 3);
            Eigen::Map<Eigen::MatrixXf> M_masses(vel.data()->data, static_cast<int>(vel.size()), 3);
        }
    }
}

auto HinaPE::FastMassSpringKernel::simulate(float dt) -> void
{
    //    auto &os = physics_system.physics_objects;
    //    for (auto &pair: os)
    //    {
    //        auto &o = pair.second;
    //        if (o->get_type() == Deformable)
    //        {
    //            auto &pos = o->get_vertices();
    //            auto &vel = o->get_velocities();
    //            auto &mass = o->get_masses();
    //            Eigen::Map<Eigen::MatrixXf> M_positions(pos.data()->data, static_cast<int>(pos.size()), 3);
    //            Eigen::Map<Eigen::MatrixXf> M_velocities(vel.data()->data, static_cast<int>(vel.size()), 3);
    //            Eigen::Map<Eigen::MatrixXf> M_masses(vel.data()->data, static_cast<int>(vel.size()), 3);
    //
    //            simulate_for_each(M_positions, M_velocities, M_positions, dt);
    //        }
    //    }
}

auto HinaPE::FastMassSpringKernel::simulate_for_each(Eigen::Map<Eigen::MatrixXf> &pos, Eigen::Map<Eigen::MatrixXf> &vel, Eigen::Map<Eigen::MatrixXf> &frc, float dt) -> void
{

}
