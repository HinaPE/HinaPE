#ifndef HINAPE_ADVECTION_SOLVER_H
#define HINAPE_ADVECTION_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "common.h"

namespace HinaPE
{
class AdvectionSolver
{
public:
	virtual void advect(
			const Geom::ScalarGrid3 &input,
			const Geom::VectorField3 &flow,
			double dt, const Geom::ScalarGrid3 *output,
			const Geom::ScalarField3 &boundarySdf/* = Geom::ConstantScalarField3(Constant::I_REAL_MAX) */) = 0;
	virtual void advect(
			const Geom::CollocatedVectorGrid3 &input,
			const Geom::VectorField3 &flow,
			double dt, Geom::CollocatedVectorGrid3 *output,
			const Geom::ScalarField3 &boundarySdf/* = Geom::ConstantScalarField3(Constant::I_REAL_MAX) */) = 0;
	virtual void advect(
			const Geom::FaceCenteredVectorGrid3 &input,
			const Geom::VectorField3 &flow,
			double dt, Geom::FaceCenteredVectorGrid3 *output,
			const Geom::ScalarField3 &boundarySdf/* = Geom::ConstantScalarField3(Constant::I_REAL_MAX) */) = 0;
};

class SemiLagrangianSolver : public AdvectionSolver
{
public:
	void advect(
			const Geom::ScalarGrid3 &input,
			const Geom::VectorField3 &flow,
			double dt,
			const Geom::ScalarGrid3 *output,
			const Geom::ScalarField3 &boundarySdf) override;
	void advect(
			const Geom::CollocatedVectorGrid3 &input,
			const Geom::VectorField3 &flow,
			double dt,
			Geom::CollocatedVectorGrid3 *output,
			const Geom::ScalarField3 &boundarySdf) override;
	void advect(
			const Geom::FaceCenteredVectorGrid3 &input,
			const Geom::VectorField3 &flow,
			double dt,
			Geom::FaceCenteredVectorGrid3 *output,
			const Geom::ScalarField3 &boundarySdf) override;
};

class CubicSemiLagrangianSolver : public SemiLagrangianSolver
{
public:
	void advect(const Geom::ScalarGrid3 &input,
				const Geom::VectorField3 &flow,
				double dt,
				const Geom::ScalarGrid3 *output,
				const Geom::ScalarField3 &boundarySdf) final;
	void advect(const Geom::CollocatedVectorGrid3 &input,
				const Geom::VectorField3 &flow,
				double dt,
				Geom::CollocatedVectorGrid3 *output,
				const Geom::ScalarField3 &boundarySdf) final;
	void advect(const Geom::FaceCenteredVectorGrid3 &input,
				const Geom::VectorField3 &flow,
				double dt,
				Geom::FaceCenteredVectorGrid3 *output,
				const Geom::ScalarField3 &boundarySdf) final;
};

using AdvectionSolverPtr = std::shared_ptr<AdvectionSolver>;
} // namespace HinaPE

#endif //HINAPE_ADVECTION_SOLVER_H
