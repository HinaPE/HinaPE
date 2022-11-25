#ifndef HINAPE_CLOTH_CLOTH_SOLVER_H
#define HINAPE_CLOTH_CLOTH_SOLVER_H

#include "../../../math_api.h"

#include <utility>

namespace HinaPE::Cloth
{
class ClothSolver
{
public:
    struct Opt
    {
        float width = 2.f;
        float height = 2.f;
        int rows = 30;
        int cols = 30;
    } _opt;

public:
    virtual void step(real dt) = 0;
    auto vertices() -> std::vector<mVector3> &;
    auto indices() -> std::vector<unsigned int> &;

protected:
    class ClothData;
    using ClothDataPtr = std::shared_ptr<ClothData>;
    ClothDataPtr _cloth_data;

public:
    explicit ClothSolver(const Opt &opt);
    ClothSolver(const ClothSolver &) = delete;
    ClothSolver(ClothSolver &&) = default;
    ~ClothSolver() = default;
    auto operator=(const ClothSolver &) -> ClothSolver & = delete;
    auto operator=(ClothSolver &&) -> ClothSolver & = default;

protected:
    void setup();
};
using ClothSolverPtr = std::shared_ptr<ClothSolver>;

class ClothSolver::ClothData
{
private:
    std::vector<mVector3> _init_vertices;
    std::vector<unsigned int> _init_indices;
    std::vector<std::pair<unsigned int, unsigned int>> _init_edges;

private:
    std::vector<mVector3> _positions;
    std::vector<mVector3> _pre_positions;
    std::vector<mVector3> _velocities;
    std::vector<mVector3> _forces;
    std::vector<real> _inv_masses;

public:
    friend class ClothSolver;
    friend class PBDSolver;
    explicit ClothData() = default;
    ClothData(const ClothData &) = delete;
    ClothData(ClothData &&) = default;
    ~ClothData() = default;
    auto operator=(const ClothData &) -> ClothData & = delete;
    auto operator=(ClothData &&) -> ClothData & = default;
};
}

#endif //HINAPE_CLOTH_CLOTH_SOLVER_H
