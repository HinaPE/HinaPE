#include "fluid_domain.h"

HinaPE::FluidDomain3::FluidDomain3()
{
	NAME = "Domain";
	_rebuild_();
	switch_surface();
	switch_bbox();
}
