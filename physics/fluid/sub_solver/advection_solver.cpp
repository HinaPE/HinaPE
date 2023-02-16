#include "advection_solver.h"
void HinaPE::SemiLagrangian3::advect(const HinaPE::Geom::ScalarGrid3 &input, const HinaPE::Geom::VectorField3 &flow, real dt, HinaPE::Geom::ScalarGrid3 *output, const HinaPE::Geom::ScalarField3 &boundary_sdf)
{
}
void HinaPE::SemiLagrangian3::advect(const HinaPE::Geom::CollocatedVectorGrid3 &input, const HinaPE::Geom::VectorField3 &flow, real dt, HinaPE::Geom::CollocatedVectorGrid3 *output, const HinaPE::Geom::ScalarField3 &boundary_sdf)
{
}
void HinaPE::SemiLagrangian3::advect(const HinaPE::Geom::FaceCenteredVectorGrid3 &input, const HinaPE::Geom::VectorField3 &flow, real dt, HinaPE::Geom::FaceCenteredVectorGrid3 *output, const HinaPE::Geom::ScalarField3 &boundary_sdf)
{
}
auto HinaPE::SemiLagrangian3::_back_trace(const HinaPE::Geom::VectorField3 &flow, real dt, real h, const mVector3 &pt0, const HinaPE::Geom::ScalarField3 &boundary_sdf) -> mVector3
{
	real remainingT = dt;
	mVector3 pt0_ = pt0;
	mVector3 pt1_ = pt0;

	while (remainingT > Constant::Epsilon)
	{
		mVector3 vel0 = flow.sample(pt0);
		real num_sub_steps = std::max(std::ceil(vel0.length() * remainingT / h), Constant::One);
		dt = remainingT / num_sub_steps;

		// mid-point rule
		mVector3 mid_pt = pt0_ - Constant::Half * vel0 * dt;
		mVector3 mid_vel = flow.sample(mid_pt);
		pt1_ = pt0_ - mid_vel * dt;

		// boundary handling
		real phi0 = boundary_sdf.sample(pt0_);
		real phi1 = boundary_sdf.sample(pt1_);

		if (phi0 * phi1 < Constant::Zero)
		{
			real w = std::fabs(phi1) / (std::fabs(phi0) + std::fabs(phi1));
			pt1_ = w * pt0 + (Constant::One - w) * pt1_;
			break;
		}

		remainingT -= dt;
		pt0_ = pt1_;
	}

	return pt1_;
}
