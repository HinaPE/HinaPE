#ifndef HINAPE_ADVECTION_SOLVER_H
#define HINAPE_ADVECTION_SOLVER_H

#include "geom/grid.h"

namespace HinaPE
{
class AdvectionSolver3
{
public:
	virtual void advect(const Geom::ScalarGrid3 &input, const Geom::VectorField3 &flow, real dt, Geom::ScalarGrid3 *output, const Geom::ScalarField3 &boundary_sdf) = 0;
	virtual void advect(const Geom::CollocatedVectorGrid3 &input, const Geom::VectorField3 &flow, real dt, Geom::CollocatedVectorGrid3 *output, const Geom::ScalarField3 &boundary_sdf) = 0;
	virtual void advect(const Geom::FaceCenteredVectorGrid3 &input, const Geom::VectorField3 &flow, real dt, Geom::FaceCenteredVectorGrid3 *output, const Geom::ScalarField3 &boundary_sdf) = 0;
};
class SemiLagrangian3 : public AdvectionSolver3
{
public:
	void advect(const Geom::ScalarGrid3 &input, const Geom::VectorField3 &flow, real dt, Geom::ScalarGrid3 *output, const Geom::ScalarField3 &boundary_sdf) final;
	void advect(const Geom::CollocatedVectorGrid3 &input, const Geom::VectorField3 &flow, real dt, Geom::CollocatedVectorGrid3 *output, const Geom::ScalarField3 &boundary_sdf) final;
	void advect(const Geom::FaceCenteredVectorGrid3 &input, const Geom::VectorField3 &flow, real dt, Geom::FaceCenteredVectorGrid3 *output, const Geom::ScalarField3 &boundary_sdf) final;

protected:
	virtual auto _get_scalar_sampler_func(const Geom::ScalarGrid3 &input) const -> std::function<real(const mVector3 &)> { return input.sampler(); }
	virtual auto _get_vector_sampler_func(const Geom::CollocatedVectorGrid3 &input) const -> std::function<mVector3(const mVector3 &)> { return input.sampler(); }
	virtual auto _get_vector_sampler_func(const Geom::FaceCenteredVectorGrid3 &input) const -> std::function<mVector3(const mVector3 &)> { return input.sampler(); }

private:
	auto _back_trace(const Geom::VectorField3 &flow, real dt, real h, const mVector3 &pt0, const Geom::ScalarField3 &boundary_sdf) -> mVector3;
};
class CubicSemiLagrangian3 : public SemiLagrangian3
{
public:
};


using AdvectionSolver3Ptr = std::shared_ptr<AdvectionSolver3>;
using SemiLagrangian3Ptr = std::shared_ptr<SemiLagrangian3>;
using CubicSemiLagrangian3Ptr = std::shared_ptr<CubicSemiLagrangian3>;
}
#endif //HINAPE_ADVECTION_SOLVER_H
