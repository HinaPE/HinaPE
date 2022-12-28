#include "sph_data.h"

void HinaPE::Fluid::SphData3::compute_mass()
{
	Array1<mVector3> points;
	BccLatticePointGenerator3 generator;
	mBBox sample_bound(mVector3(-1.5f * _opt.kernel_radius, -1.5f * _opt.kernel_radius, -1.5f * _opt.kernel_radius), mVector3(1.5f * _opt.kernel_radius, 1.5f * _opt.kernel_radius, 1.5f * _opt.kernel_radius));

	generator.generate(sample_bound, _opt.target_spacing, &points);
}
