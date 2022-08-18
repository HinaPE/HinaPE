#ifndef HINAPE_FMS_KERNEL_H
#define HINAPE_FMS_KERNEL_H

#include "../../common.h"

#include <map>

namespace HinaPE
{
class PhysicsSystem;

class FastMassSpringKernel final
{
    using CholeskySolver = Eigen::SimplicialCholesky<Eigen::SparseMatrix<float>>;
    using SparseMatrix = Eigen::SparseMatrix<float>;
    using TripletList = std::vector<Eigen::Triplet<float>>;

public:
    auto simulate(float dt) -> void;

    struct Opt
    {
        Eigen::Vector3f gravity;
        Eigen::Vector3f wind_direction;
        Eigen::Vector3f wind_speed;
        float fixed_dt = 1 / 30.f;
    };
    Opt opt;

public:
    explicit FastMassSpringKernel(PhysicsSystem &sys);

private:
    auto simulate_for_each(Eigen::Map<Eigen::MatrixXf> &pos, Eigen::Map<Eigen::MatrixXf> &vel, Eigen::Map<Eigen::MatrixXf> &frc, float dt) -> void;

private:
    PhysicsSystem &physics_system;

    // first: cloth id, second: cached sparse matrix
    std::map<unsigned int, SparseMatrix> Ms_cached;
    std::map<unsigned int, SparseMatrix> Ls_cached;
    std::map<unsigned int, SparseMatrix> Js_cached;
};
}
#endif //HINAPE_FMS_KERNEL_H
