#ifndef HINAPE_SPH_DATA_H
#define HINAPE_SPH_DATA_H

#include "../particle_system_data3.h"
#include "_point_generator/point_generator3.h"

#include <limits>

namespace HinaPE::Fluid
{
class SphData3 : public ParticleSystemData3
{
public:
	struct Opt
	{
		real target_density = 1000.0;
		real target_spacing = 0.1;
		real kernel_radius_over_target_spacing = 1.8;
		real kernel_radius = std::numeric_limits<real>::min();
		size_t pressure_idx = std::numeric_limits<size_t>::max();
		size_t density_idx = std::numeric_limits<size_t>::max();
	} _opt;

private:
	void compute_mass();
};
}

#endif //HINAPE_SPH_DATA_H
