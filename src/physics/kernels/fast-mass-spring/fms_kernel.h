#ifndef HINAPE_FMS_KERNEL_H
#define HINAPE_FMS_KERNEL_H

#include "../../common.h"

#include <map>

namespace HinaPE
{
class PhysicsSystem;

    struct mass_spring_system {
        // EdgeList spring_list;        // spring edge list
        Eigen::VectorXf rest_lengths;       // spring rest lengths
        Eigen::VectorXf stiffnesses;       // spring stiffnesses
        Eigen::VectorXf fext;               // external forces
    };

class FastMassSpringKernel final
{
    using CholeskySolver = Eigen::SimplicialCholesky<Eigen::SparseMatrix<float>>;
    using SparseMatrix = Eigen::SparseMatrix<float>;
    using TripletList = std::vector<Eigen::Triplet<float>>;

public:
    auto init() -> void;
    auto simulate(float dt) -> void;

    void spring_deformation_constraint(float rest_length);
    void sphere_collision_constraint(float radius, Eigen::Vector3f center);


    struct Opt
    {
        Eigen::Vector3f gravity;
        Eigen::Vector3f wind_direction;
        Eigen::Vector3f wind_speed;
        float damping_factor = 0.993f;         // damping factor
        float fixed_dt = 1 / 30.f;
    };
    Opt opt;

    //typedef std::pair<unsigned int, unsigned int> Edge;
    //typedef std::vector<Edge> EdgeList;

    mass_spring_system* system;

    Eigen::Vector3f current_state; // q(n), current state
    Eigen::Vector3f prev_state; // q(n - 1), previous state
    Eigen::Vector3f spring_directions; // d, spring directions
    Eigen::Vector3f inertial_term; // M * y, y = (a + 1) * q(n) - a * q(n - 1)

public:
    explicit FastMassSpringKernel(PhysicsSystem &sys);

private:
    auto simulate_for_each(Eigen::Map<Eigen::MatrixXf> &pos, Eigen::Map<Eigen::MatrixXf> &vel, Eigen::Map<Eigen::MatrixXf> &frc, float dt) -> void;

private:
    PhysicsSystem &physics_system;
    bool inited = false;

    // first: cloth id, second: cached sparse matrix
    std::map<unsigned int, SparseMatrix> Ms_cached;
    std::map<unsigned int, SparseMatrix> Ls_cached;
    std::map<unsigned int, SparseMatrix> Js_cached;
    std::map<unsigned int, std::shared_ptr<CholeskySolver>> solver_cached;

private:
    typedef std::vector<unsigned int> IndexList;
    IndexList structI, shearI, bendI;
    mass_spring_system* result;
public:
    void uniformGrid(
            unsigned int gird_width,          // grid width
            float rest_length,       // spring rest length (non-diagonal)
            float stiffness,         // spring stiffness
            float gravity            // gravitationl force (-z axis)
    );

    IndexList getStructIndex(); // structural springs
    IndexList getShearIndex(); // shearing springs
    IndexList getBendIndex(); // bending springs

    mass_spring_system* getResult();
protected:
    std::unordered_map<unsigned int, Eigen::Vector3f> fix_map;
    virtual void fixPoint(unsigned int i);
    virtual void releasePoint(unsigned int i);
    bool is_fixed();
};
}
#endif //HINAPE_FMS_KERNEL_H
