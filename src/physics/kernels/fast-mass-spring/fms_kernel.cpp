#include <iostream>
#include "fms_kernel.h"
#include "../../physics_system.h"

HinaPE::FastMassSpringKernel::FastMassSpringKernel(HinaPE::PhysicsSystem &sys) : physics_system(sys) {}

auto HinaPE::FastMassSpringKernel::init() -> void
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
                    LTriplets.push_back(Eigen::Triplet<float>(3 * edge.first + i, 3 * edge.second + i, 1.0f));
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
                    JTriplets.push_back(Eigen::Triplet<float>(3 * edge.first + i, 3 * k + i, 1.0f));
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
            system_matrix->compute(A);

            Ls_cached[id] = L;
            Js_cached[id] = J;
            Ms_cached[id] = M;
            solver_cached[id] = system_matrix;

            current_state.resize(3 * vertices_num);
            current_state.setZero();
            prev_state.resize(3 * vertices_num);
            prev_state.setZero();
            spring_directions.resize(3 * edges_num);
            spring_directions.setZero();
            inertial_term.resize(3 * vertices_num);
            inertial_term.setZero();

        }
    }
    inited = true;
}

auto HinaPE::FastMassSpringKernel::simulate(float dt) -> void
{
    if (!inited)
        init();

    float a = opt.damping_factor; // shorthand
    auto &os = physics_system.physics_objects;
    for (auto &pair: os){
        auto id = pair.first;
        auto &o = pair.second->get_object<DeformableBase<CLOTH>>();

        auto &verts = o.vertices();
        auto &edges = o.edges();
        auto &masses = o.masses();
        Eigen::Map<Eigen::MatrixXf> M_vertices(verts.data()->data, static_cast<int>(verts.size()), 3);
        auto vertices_num = M_vertices.rows();
        auto edges_num = (int) edges.size();

        /////////// set constraint

        /// sphere_collision_constraint
        float radius = 0.64f;
        Vec3 center(0.0f,0.0f,0.0f);
        for (int i = 0; i < vertices_num; i++){
            Vec3 p(
                    verts[i].x-center.x,
                    verts[i].y-center.y,
                    verts[i].z-center.z
            );
            if (p.norm() < radius) {
                p.normalize();
                p = radius * p;
            }
            else continue;

            verts[i].x = p.x + center.x;
            verts[i].y = p.y + center.y;
            verts[i].z = p.z + center.z;
        }
        ///

        /// spring_deformation_constraint
        for (auto &edge : edges) {
            Vec3 p12(
                    verts[edge.first].x - verts[edge.second].x,
                    verts[edge.first].y - verts[edge.second].y,
                    verts[edge.first].z - verts[edge.second].z
            );

            float len = p12.norm();
            float tauc = 0.4f; // critical deformation rate
            float rlen = 0.07;
            float diff = (len - (1 + tauc) * rlen) / len;
            float rate = (len - rlen) / rlen;

            if (rate <= tauc) continue;

            float f1, f2;
            f1 = f2 = 0.5f;

            // if first point is fixed
            if((edge.first == vertices_num - 1) || (edge.first == 0))
            {
                f1 = 0.0f; f2 = 1.0f;
            }

            // if second point is fixed
            if((edge.second == 0) || (edge.second == vertices_num - 1))
            {
                f1 = (f1 != 0.0f ? 1.0f : 0.0f);
                f2 = 0.0f;
            }

            verts[edge.first].x -= p12.x * f1 * diff;
            verts[edge.first].y -= p12.y * f1 * diff;
            verts[edge.first].z -= p12.z * f1 * diff;

            verts[edge.second].x += p12.x * f2 * diff;
            verts[edge.second].y += p12.y * f2 * diff;
            verts[edge.second].z += p12.z * f2 * diff;
        }
        ///

//        for (int i = 0; i < vertices_num; i++){

//          std::cout << verts[0].x << std::endl;
//          std::cout << verts[0].y << std::endl;
//          std::cout << verts[0].z << std::endl;
//          std::cout << std::endl;

//          verts[i].x+=0.01;
//          verts[i].y+=0.01;
//           verts[i].z+=0.001;

//          std::cout << verts[0].x << std::endl;
//          std::cout << verts[0].y << std::endl;
//          std::cout << verts[0].z << std::endl;
//          std::cout << std::endl;
//        }

        /// animate cloth
        for(int i = 0; i < vertices_num; i++){
            current_state[3 * i + 0] = verts[i].x;
            current_state[3 * i + 1] = verts[i].y;
            current_state[3 * i + 2] = verts[i].z;
        }
//        prev_state[0] = current_state[0];
        inertial_term = Ms_cached[id] * ((a + 1) * (current_state) - a * prev_state);

        prev_state = current_state;

        float rest_lengths = 0.07;

        for (unsigned int i = 0; i < 10; i++){
            // localStep(id);
            unsigned int j = 0;
            for (auto &edge : edges) {
                Vec3 p12(
                        current_state[3 * edge.first + 0] - current_state[3 * edge.second + 0],
                        current_state[3 * edge.first + 1] - current_state[3 * edge.second + 1],
                        current_state[3 * edge.first + 2] - current_state[3 * edge.second + 2]
                );

                p12.normalize();
                spring_directions[3 * j + 0] = rest_lengths * p12.x;
                spring_directions[3 * j + 1] = rest_lengths * p12.y;
                spring_directions[3 * j + 2] = rest_lengths * p12.z;
                j++;
            }
            // globalStep(id);
            float gravity = 9.8f * 0.25f / (31 * 31);
            Eigen::VectorXf fext = Eigen::Vector3f(0, 0, -gravity).replicate(vertices_num, 1);

            float h2 = opt.fixed_dt * opt.fixed_dt; // shorthand
            Eigen::VectorXf b = inertial_term
                                + h2 * Js_cached[id] * spring_directions
                                + h2 * fext;

            // solve system and update state
            current_state = solver_cached[id]->solve(b);
            for(int i = 0; i < vertices_num; i++){
                verts[i].x = current_state[3 * i + 0];
                verts[i].y = current_state[3 * i + 1];
                verts[i].z = current_state[3 * i + 2];
            }
        }
    }
//    //    auto &os = physics_system.physics_objects;
//    //    for (auto &pair: os)
//    //    {
//    //        auto &o = pair.second;
//    //        if (o->get_type() == Deformable)
//    //        {
//    //            auto &pos = o->get_vertices();
//    //            auto &vel = o->get_velocities();
//    //            auto &mass = o->get_masses();
//    //            Eigen::Map<Eigen::MatrixXf> M_positions(pos.data()->data, static_cast<int>(pos.size()), 3);
//    //            Eigen::Map<Eigen::MatrixXf> M_velocities(vel.data()->data, static_cast<int>(vel.size()), 3);
//    //            Eigen::Map<Eigen::MatrixXf> M_masses(vel.data()->data, static_cast<int>(vel.size()), 3);
//    //
//    //            simulate_for_each(M_positions, M_velocities, M_positions, dt);
//    //        }
//    //    }
}

auto HinaPE::FastMassSpringKernel::simulate_for_each(Eigen::Map<Eigen::MatrixXf> &pos, Eigen::Map<Eigen::MatrixXf> &vel, Eigen::Map<Eigen::MatrixXf> &frc, float dt) -> void
{

}

void HinaPE::FastMassSpringKernel::uniformGrid(unsigned int n, float rest_length, float stiffness,
                                               float gravity) {
    auto &os = physics_system.physics_objects;
    for (auto &pair: os)
    {
        if (pair.second->is<DeformableBase<CLOTH>>())
        {
            auto id = pair.first;
            auto &o = pair.second->get_object<DeformableBase<CLOTH>>();

            ///////////////////
            //如果所有弹簧的rest_length stiffness 相同的话就不需要这个了
            // n must be odd
            assert(n % 2 == 1);

            // shorthand
            const double root2 = 1.41421356237;

            auto &verts = o.vertices();
            auto &edges = o.edges();
            auto &masses = o.masses();
            Eigen::Map<Eigen::MatrixXf> M_vertices(verts.data()->data, static_cast<int>(verts.size()), 3);
            auto vertices_num = M_vertices.rows();
            auto edges_num = (int) edges.size();

            structI.reserve(2 * (n - 1) * n);
            shearI.reserve(2 * (n - 1) * (n - 1));
            bendI.reserve(n * (n - 1));

            system->rest_lengths(edges_num);
            system->stiffnesses(edges_num);

            unsigned int k = 0; // spring counter
            for(unsigned int i = 0; i < n; i++) {
                for(unsigned int j = 0; j < n; j++) {
                    if(i == n - 1 && j == n - 1) {
                        continue;
                    }
                    if (i == n - 1) {
                        float width = 2.0f;
                        rest_length = width / (n - 1) * 1.05f;
                        system->rest_lengths[k] = rest_length;
                        system->stiffnesses[k] = stiffness;
                        structI.push_back(k++);

                        if (j % 2 == 0) {
                            system->rest_lengths[k] = 2 * rest_length;
                            system->stiffnesses[k] = stiffness;
                            bendI.push_back(k++);
                        }
                        continue;
                    }
                    if (j == n - 1) {
                        // structural spring
                        system->rest_lengths[k] = rest_length;
                        system->stiffnesses[k] = stiffness;
                        structI.push_back(k++);

                        // bending spring
                        if (i % 2 == 0){
                            system->rest_lengths[k] = 2 * rest_length;
                            system->stiffnesses[k] = stiffness;
                            bendI.push_back(k++);
                        }
                        continue;
                    }
                    system->rest_lengths[k] = rest_length;
                    system->stiffnesses[k] = stiffness;
                    structI.push_back(k++);

                    system->rest_lengths[k] = rest_length;
                    system->stiffnesses[k] = stiffness;
                    structI.push_back(k++);

                    system->rest_lengths[k] = root2 * rest_length;
                    system->stiffnesses[k] = stiffness;
                    shearI.push_back(k++);

                    system->rest_lengths[k] = root2 * rest_length;
                    system->stiffnesses[k] = stiffness;
                    shearI.push_back(k++);

                    if (j % 2 == 0) {
                        system->rest_lengths[k] = 2 * rest_length;
                        system->stiffnesses[k] = stiffness;
                        bendI.push_back(k++);
                    }
                    if (i % 2 == 0) {
                        system->rest_lengths[k] = 2 * rest_length;
                        system->stiffnesses[k] = stiffness;
                        bendI.push_back(k++);
                    }
                }
            }

            Eigen::VectorXf fext = Eigen::Vector3f(0, 0, -gravity).replicate(vertices_num, 1);

            result->rest_lengths = system->rest_lengths;
            result->stiffnesses = system->stiffnesses;
            result->fext = fext;
        }
    }
}

HinaPE::FastMassSpringKernel::IndexList HinaPE::FastMassSpringKernel::getStructIndex() {
    return structI;
}

HinaPE::FastMassSpringKernel::IndexList HinaPE::FastMassSpringKernel::getShearIndex() {
    return shearI;
}

HinaPE::FastMassSpringKernel::IndexList HinaPE::FastMassSpringKernel::getBendIndex() {
    return bendI;
}

HinaPE::mass_spring_system *HinaPE::FastMassSpringKernel::getResult() {
    return result;
}

void HinaPE::FastMassSpringKernel::spring_deformation_constraint(float rest_length) {
    for (int k = 0; k < 15; k++){
        auto &os = physics_system.physics_objects;
        for (auto &pair: os) {
            if (pair.second->is<DeformableBase<CLOTH>>()) {
                auto id = pair.first;
                auto &o = pair.second->get_object<DeformableBase<CLOTH>>();

                auto &verts = o.vertices();
                auto &edges = o.edges();
                auto &masses = o.masses();
                Eigen::Map<Eigen::MatrixXf> M_vertices(verts.data()->data, static_cast<int>(verts.size()), 3);
                auto vertices_num = M_vertices.rows();
                auto edges_num = (int) edges.size();

                for (auto &edge : edges) {
                    Eigen::Vector3f p12(
                            verts[edge.first].x - verts[edge.second].x,
                            verts[edge.first].y - verts[edge.second].y,
                            verts[edge.first].z - verts[edge.second].z
                    );
                    float tauc = 0.4f; // critical deformation rate
                    float len = p12.norm();
                    float rlen = rest_length;
                    float diff = (len - (1 + tauc) * rlen) / len;
                    float rate = (len - rlen) / rlen;

                    if (rate <= tauc) continue;

                    float f1, f2;
                    f1 = f2 = 0.5f;

                    // if first point is fixed
                    if((edge.first = vertices_num - 1) || (edge.first = 0))
                    {
                        f1 = 0.0f; f2 = 1.0f;
                    }
                    // if second point is fixed
                    if((edge.second != 0) || (edge.second != vertices_num - 1))
                    {
                        f1 = (f1 != 0.0f ? 1.0f : 0.0f);
                        f2 = 0.0f;
                    }

                    verts[edge.first].x -= p12[0] * f1 * diff;
                    verts[edge.first].y -= p12[1] * f1 * diff;
                    verts[edge.first].z -= p12[2] * f1 * diff;

                    verts[edge.second].x += p12[0] * f2 * diff;
                    verts[edge.second].y += p12[1] * f2 * diff;
                    verts[edge.second].z += p12[2] * f2 * diff;
                }
            }
        }
    }
}

void HinaPE::FastMassSpringKernel::sphere_collision_constraint(float radius, Eigen::Vector3f center) {
    auto &os = physics_system.physics_objects;
    for (auto &pair: os) {
        if (pair.second->is<DeformableBase<CLOTH>>()) {
            auto id = pair.first;
            auto &o = pair.second->get_object<DeformableBase<CLOTH>>();

            auto &verts = o.vertices();
            auto &edges = o.edges();
            auto &masses = o.masses();
            Eigen::Map<Eigen::MatrixXf> M_vertices(verts.data()->data, static_cast<int>(verts.size()), 3);
            auto vertices_num = M_vertices.rows();
            auto edges_num = (int) edges.size();

            for (int i = 0; i < vertices_num; i++){
                Eigen::Vector3f p(
                        (verts[i]-center[0]).x,
                        (verts[i]-center[0]).y,
                        (verts[i]-center[0]).z
                    );

                if (p.norm() < radius) {
                    p.normalize();
                    p = radius * p;
                }
                else continue;

                verts[i].x = p[0] + center[0];
                verts[i].y = p[1] + center[1];
                verts[i].z = p[2] + center[2];
            }
        }
    }
}

void HinaPE::FastMassSpringKernel::fixPoint(unsigned int i) {
    auto &os = physics_system.physics_objects;
    for (auto &pair: os) {
        if (pair.second->is<DeformableBase<CLOTH>>()) {
            auto id = pair.first;
            auto &o = pair.second->get_object<DeformableBase<CLOTH>>();

            auto &verts = o.vertices();

            fix_map[3 * i] = Eigen::Vector3f(verts[i].x, verts[i].y, verts[i].z);
        }
    }
}

void HinaPE::FastMassSpringKernel::releasePoint(unsigned int i) {
    fix_map.erase(3 * i);
}

bool HinaPE::FastMassSpringKernel::is_fixed() {
     //0 n-1
     return false;
}



