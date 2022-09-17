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
    CholeskySolver system_matrix;

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

};

////////////////////////////

// Constraint Graph
class CgNodeVisitor; // Constraint graph node visitor

// Constraint graph node
class CgNode {
protected:
    mass_spring_system* system;
    float* vbuff;

public:
    explicit CgNode(mass_spring_system* system);

    virtual void satisfy() = 0; // satisfy constraint
    virtual bool accept(CgNodeVisitor& visitor) = 0; // accept visitor
};

class CgPointNode : public CgNode {
    public:
        explicit CgPointNode(mass_spring_system* system);
        [[nodiscard]] virtual bool query(unsigned int i) const = 0; // check if point with index i is constrained
        bool accept(CgNodeVisitor& visitor) override;
};

// spring constraint node
class CgSpringNode : public CgNode {
protected:
    typedef std::vector<CgNode*> NodeList;
    NodeList children;

public:
    explicit CgSpringNode(mass_spring_system* system);

    bool accept(CgNodeVisitor& visitor) override;
    void addChild(CgNode* node);
    void removeChild(CgNode* node);
};

// root node
class CgRootNode : public CgSpringNode {
public:
    CgRootNode(mass_spring_system* system);

    void satisfy() override;
    bool accept(CgNodeVisitor& visitor) override;
};



}
#endif //HINAPE_FMS_KERNEL_H
