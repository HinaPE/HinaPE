#ifndef HINAPE_PBD_SOLVER_H
#define HINAPE_PBD_SOLVER_H

#include "../cloth_solver.h"
#include <memory>

namespace HinaPE::ClothEngine
{
class PBDSolver : public ClothSolver
{
public:
    class Builder;
    static auto builder() -> Builder;
    void print_hello();

protected:
    PBDSolver();
    void on_begin_advance_time_step(double time_interval_in_seconds) override;
    void on_end_advance_time_step(double time_interval_in_seconds) override;
};
using PBDSolverPtr = std::shared_ptr<PBDSolver>;

template<typename DerivedBuilder>
class PBDSolverBuilderBase : public ClothSolverBuilderBase<DerivedBuilder>
{
public:
    auto with_stiffness(double _stiffness) -> DerivedBuilder &;

protected:
    double stiffness = 1.0;
};
template<typename DerivedBuilder>
auto PBDSolverBuilderBase<DerivedBuilder>::with_stiffness(double _stiffness) -> DerivedBuilder &
{
    stiffness = _stiffness;
    return static_cast<DerivedBuilder &>(*this);
}

class PBDSolver::Builder final : public PBDSolverBuilderBase<PBDSolver::Builder>
{
public:
    auto build() const -> PBDSolver; // NOT USED
    auto makeShared() const -> PBDSolverPtr;
};
}

#endif //HINAPE_PBD_SOLVER_H
