#include "fluid_domain.h"

HinaPE::FluidDomain3::FluidDomain3()
{
	NAME = "Domain";
	Kasumi::CubeObject::sync_opt();
	_switch_surface();
	_switch_bbox();
}
