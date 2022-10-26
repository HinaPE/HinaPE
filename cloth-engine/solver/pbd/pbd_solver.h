#ifndef HINAPE_PBD_SOLVER_H
#define HINAPE_PBD_SOLVER_H

#include <memory>

namespace HinaPE::ClothEngine
{
class PBDSolver
{
private:
    class Builder;

public:
    void update(double dt);

    static auto builder() -> Builder;

};
using PBDSolverPtr = std::shared_ptr<PBDSolver>;

// ==================== Builder ====================
template<typename DerivedBuilder>
class PBDSolverBuilderBase
{
public:
    auto withStiffness(double _stiffness) -> DerivedBuilder &;
protected:
    double stiffness = 1.0;
};

template<typename DerivedBuilder>
auto PBDSolverBuilderBase<DerivedBuilder>::withStiffness(double _stiffness) -> DerivedBuilder &
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
