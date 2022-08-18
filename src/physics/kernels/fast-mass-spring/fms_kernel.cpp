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
            auto &edges = o.edges();
            auto &masses = o.masses();
            Eigen::Map<Eigen::MatrixXf> M_vertices(verts.data()->data, static_cast<int>(verts.size()), 3);
            auto vertices_num = M_vertices.rows();
            auto edges_num = (int) edges.size();

            TripletList LTriplets;
            SparseMatrix L;
            L.resize(3 * vertices_num, 3 * vertices_num);
            unsigned int k = 0;
            for (auto &edge: edges)
            {
                for (int i = 0; i < 3; ++i)
                {
                    LTriplets.push_back(Eigen::Triplet<float>(3 * edge.first + i, 3 * edge.second + i, 1.0f));/* TODO: initial stiffness here */
                    LTriplets.push_back(Eigen::Triplet<float>(3 * edge.first + i, 3 * edge.second + i, -1.0f));
                    LTriplets.push_back(Eigen::Triplet<float>(3 * edge.second + i, 3 * edge.first + i, -1.0f));
                    LTriplets.push_back(Eigen::Triplet<float>(3 * edge.second + i, 3 * edge.first + i, 1.0f));
                }
                ++k;
            }
            L.setFromTriplets(LTriplets.begin(), LTriplets.end());

            TripletList JTriplets;
            SparseMatrix J;
            J.resize(3 * vertices_num, 3 * edges_num);
            k = 0;
            for (auto &edge: edges)
            {
                for (int i = 0; i < 3; ++i)
                {
                    JTriplets.push_back(Eigen::Triplet<float>(3 * edge.first + i, 3 * k + i, 1.0f));/* TODO: initial stiffness here */
                    JTriplets.push_back(Eigen::Triplet<float>(3 * edge.second + i, 3 * k + i, 1.0f));
                }
                ++k;
            }
            J.setFromTriplets(JTriplets.begin(), JTriplets.end());

            TripletList MTriplets;
            SparseMatrix M;
            M.resize(3 * vertices_num, 3 * vertices_num);
            k = 0;
            for (auto &mass: masses)
            {
                for (int i = 0; i < 3; ++i)
                {
                    MTriplets.push_back(Eigen::Triplet<float>(3 * k + i, 3 * k + i, mass));
                }
                ++k;
            }
            M.setFromTriplets(MTriplets.begin(), MTriplets.end());

            SparseMatrix A = M + opt.fixed_dt * opt.fixed_dt * L;
            auto system_matrix = std::make_shared<CholeskySolver>();
            system_matrix->solve(A);

            Ls_cached[id] = L;
            Js_cached[id] = J;
            Ms_cached[id] = M;
            solver_cached[id] = system_matrix;
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
