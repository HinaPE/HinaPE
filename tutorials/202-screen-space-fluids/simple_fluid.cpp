#include "simple_fluid.h"

HinaPE::SimpleFluid::SimpleFluid()
{
	_data = std::make_shared<Data>();
}

HinaPE::SimpleFluid::Data::Data()
{
	Fluid.positions = Util::sphere_particles();
}
