#include "advection_solver.h"

void HinaPE::SemiLagrangianSolver::advect(const HinaPE::Geom::ScalarGrid3 &input, const HinaPE::Geom::VectorField3 &flow, double dt, const HinaPE::Geom::ScalarGrid3 *output, const HinaPE::Geom::ScalarField3 &boundarySdf)
{
}
void HinaPE::SemiLagrangianSolver::advect(const HinaPE::Geom::CollocatedVectorGrid3 &input, const HinaPE::Geom::VectorField3 &flow, double dt, HinaPE::Geom::CollocatedVectorGrid3 *output, const HinaPE::Geom::ScalarField3 &boundarySdf)
{
}
void HinaPE::SemiLagrangianSolver::advect(const HinaPE::Geom::FaceCenteredVectorGrid3 &input, const HinaPE::Geom::VectorField3 &flow, double dt, HinaPE::Geom::FaceCenteredVectorGrid3 *output, const HinaPE::Geom::ScalarField3 &boundarySdf)
{
}

void HinaPE::CubicSemiLagrangianSolver::advect(const HinaPE::Geom::ScalarGrid3 &input, const HinaPE::Geom::VectorField3 &flow, double dt, const HinaPE::Geom::ScalarGrid3 *output, const HinaPE::Geom::ScalarField3 &boundarySdf) {}
void HinaPE::CubicSemiLagrangianSolver::advect(const HinaPE::Geom::CollocatedVectorGrid3 &input, const HinaPE::Geom::VectorField3 &flow, double dt, HinaPE::Geom::CollocatedVectorGrid3 *output, const HinaPE::Geom::ScalarField3 &boundarySdf) {}
void HinaPE::CubicSemiLagrangianSolver::advect(const HinaPE::Geom::FaceCenteredVectorGrid3 &input, const HinaPE::Geom::VectorField3 &flow, double dt, HinaPE::Geom::FaceCenteredVectorGrid3 *output, const HinaPE::Geom::ScalarField3 &boundarySdf) {}
