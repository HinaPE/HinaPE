#include "fluid_domain.h"

HinaPE::FluidDomain3::FluidDomain3()
{
	NAME = "Domain";
	Kasumi::CubeObject::sync_opt();
	switch_surface();
	switch_bbox();
}
