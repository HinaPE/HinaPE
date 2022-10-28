#ifndef HINAPE_CLOTH_ENGINE_CLOTH_SOLVER_H
#define HINAPE_CLOTH_ENGINE_CLOTH_SOLVER_H

#include "animation/physics_animation.h"
#include "cloth_data.h"

namespace HinaPE::ClothEngine
{
class ClothSolver : HinaPE::PhysicsAnimation
{
public:
    class Builder;
    static auto builder() -> Builder;

protected:
    ClothSolver();
//    ClothSolver(double width, double height, int rows, int cols, const Vector3D& center_point, const Vector3D &orientation);
    void onInitialize() override;
    void onAdvanceTimeStep(double timeIntervalInSeconds) final;
    virtual void on_begin_advance_time_step(double time_interval_in_seconds); // Empty, to be overridden
    virtual void on_end_advance_time_step(double time_interval_in_seconds); // Empty, to be overridden

private:
    ClothDataPtr _cloth_data;
    ClothData::VectorData _new_positions;
    ClothData::VectorData _new_velocities;
};
using ClothSolverPtr = std::shared_ptr<ClothSolver>;

template<typename DerivedBuilder>
class ClothSolverBuilderBase
{
public:
    auto with_size(double width, double height) -> DerivedBuilder &;
    auto with_dimension(double rows, double cols) -> DerivedBuilder &;
    auto with_position(const Vector3D &position) -> DerivedBuilder &;
    auto with_orientation(const Vector3D &orientation) -> DerivedBuilder &;

private:
    double _width = 1, _height = 1;
    double _rows = 20, _cols = 20;
    Vector3D _position = {0, 0, 0};
    Vector3D _orientation = {0, 0, 1};
};
template<typename DerivedBuilder>
auto ClothSolverBuilderBase<DerivedBuilder>::with_size(double width, double height) -> DerivedBuilder &
{
    _width = width;
    _height = height;
    return static_cast<DerivedBuilder &>(*this);
}
template<typename DerivedBuilder>
auto ClothSolverBuilderBase<DerivedBuilder>::with_dimension(double rows, double cols) -> DerivedBuilder &
{
    _rows = rows;
    _cols = cols;
    return static_cast<DerivedBuilder &>(*this);
}
template<typename DerivedBuilder>
auto ClothSolverBuilderBase<DerivedBuilder>::with_position(const Vector3D &position) -> DerivedBuilder &
{
    _position = position;
    return static_cast<DerivedBuilder &>(*this);
}
template<typename DerivedBuilder>
auto ClothSolverBuilderBase<DerivedBuilder>::with_orientation(const Vector3D &orientation) -> DerivedBuilder &
{
    _orientation = orientation;
    return static_cast<DerivedBuilder &>(*this);
}

class ClothSolver::Builder final : public ClothSolverBuilderBase<ClothSolver::Builder>
{
public:
    auto build() const -> ClothSolver;
    auto make_shared() const -> ClothSolverPtr;
};
}

#endif //HINAPE_CLOTH_ENGINE_CLOTH_SOLVER_H
