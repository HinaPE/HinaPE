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

            current_state(vertices_num * 3);
            prev_state(current_state);
            spring_directions(edges_num * 3);

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
            system_matrix->compute(A);

            Ls_cached[id] = L;
            Js_cached[id] = J;
            Ms_cached[id] = M;
            solver_cached[id] = system_matrix;
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
        auto &edges = o.edges();
        inertial_term = Ms_cached[id] * ((a + 1) * (current_state) - a * prev_state);
        prev_state = current_state;
        for (unsigned int i = 0; i < 10; i++) {

            // localStep(id);
            unsigned int j = 0;
            for (auto &edge : edges) {
                Eigen::Vector3f p12(
                        current_state[3 * edge.first + 0] - current_state[3 * edge.second + 0],
                        current_state[3 * edge.first + 1] - current_state[3 * edge.second + 1],
                        current_state[3 * edge.first + 2] - current_state[3 * edge.second + 2]
                );

                p12.normalize();
                spring_directions[3 * j + 0] = 	system->rest_lengths[j] * p12[0];
                spring_directions[3 * j + 1] =	system->rest_lengths[j] * p12[1];
                spring_directions[3 * j + 2] =	system->rest_lengths[j] * p12[2];
                j++;
            }

            // globalStep(id);
            float h2 = opt.fixed_dt * opt.fixed_dt; // shorthand

            // compute right hand side
            Eigen::VectorXf b = inertial_term
                                + h2 * Js_cached[id] * spring_directions
                                + h2 * system->fext;

            // solve system and update state
            current_state = system_matrix.solve(b);

        }
    }
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

HinaPE::CgNode::CgNode(HinaPE::mass_spring_system *system) : system(system){

}

HinaPE::CgPointNode::CgPointNode(HinaPE::mass_spring_system *system) : CgNode(system) {

}

bool HinaPE::CgPointNode::accept(HinaPE::CgNodeVisitor &visitor) {
    return visitor.visit(*this);
}

HinaPE::CgSpringNode::CgSpringNode(HinaPE::mass_spring_system *system) : CgNode(system) {

}

void HinaPE::CgSpringNode::addChild(HinaPE::CgNode *node) {
    children.push_back(node);
}

bool HinaPE::CgSpringNode::accept(HinaPE::CgNodeVisitor &visitor) {
    for (CgNode* child : children) {
        if (!child->accept(visitor)) return false;
    }
    return visitor.visit(*this);
}

void HinaPE::CgSpringNode::removeChild(HinaPE::CgNode *node) {
    children.erase(find(children.begin(), children.end(), node));
}

void HinaPE::CgRootNode::satisfy() {
    }

HinaPE::CgRootNode::CgRootNode(HinaPE::mass_spring_system *system) : CgSpringNode(system){

}

bool HinaPE::CgRootNode::accept(HinaPE::CgNodeVisitor &visitor) {
    for (CgNode* child : children) {
        if (!child->accept(visitor)) return false;
    }
    return true;
}
