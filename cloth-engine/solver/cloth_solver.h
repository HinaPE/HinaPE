#ifndef HINAPE_CLOTH_SOLVER_H
#define HINAPE_CLOTH_SOLVER_H

#include "animation/physics_animation.h"
#include "cloth_data.h"

namespace HinaPE::ClothEngine
{
class ClothSolver : HinaPE::PhysicsAnimation
{
public:
    class Builder;
    ClothSolver();

protected:
    void onInitialize() override;
    void onAdvanceTimeStep(double timeIntervalInSeconds) override;

private:
    ClothDataPtr _cloth_data;
    ClothData::VectorData _new_positions;
    ClothData::VectorData _new_velocities;
};
using ClothSolverPtr = std::shared_ptr<ClothSolver>;

template<typename DerivedBuilder>
class ClothSolverBuilderBase3
{
public:
    auto with_size(double width, double height) -> DerivedBuilder &;
    auto with_dimension(double rows, double cols, double depth) -> DerivedBuilder &;
    auto with_position(const Vector3D &position) -> DerivedBuilder &;
    auto with_orientation(const Vector3D &orientation) -> DerivedBuilder &;

private:
    double _width = 1, _height = 1;
    double _rows = 20, _cols = 20;
    Vector3D _position = {0, 0, 0};
    Vector3D _orientation = {0, 0, 1};
};
template<typename DerivedBuilder>
auto ClothSolverBuilderBase3<DerivedBuilder>::with_size(double width, double height) -> DerivedBuilder &
{
    _width = width;
    _height = height;
    return static_cast<DerivedBuilder &>(*this);
}
template<typename DerivedBuilder>
auto ClothSolverBuilderBase3<DerivedBuilder>::with_dimension(double rows, double cols, double depth) -> DerivedBuilder &
{
    _rows = rows;
    _cols = cols;
    return static_cast<DerivedBuilder &>(*this);
}
template<typename DerivedBuilder>
auto ClothSolverBuilderBase3<DerivedBuilder>::with_position(const Vector3D &position) -> DerivedBuilder &
{
    _position = position;
    return static_cast<DerivedBuilder &>(*this);
}
template<typename DerivedBuilder>
auto ClothSolverBuilderBase3<DerivedBuilder>::with_orientation(const Vector3D &orientation) -> DerivedBuilder &
{
    _orientation = orientation;
    return static_cast<DerivedBuilder &>(*this);
}

class ClothSolver::Builder final : public ClothSolverBuilderBase3<ClothSolver::Builder>
{
public:
    static auto build() -> ClothSolver;
    static auto make_shared() -> ClothSolverPtr;

private:
    static void create_grid(const Vector3D &center_point, const Vector3D &orientation, double width, double height, int rows, int cols);
};
}

#endif //HINAPE_CLOTH_SOLVER_H
