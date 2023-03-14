#include "sphere_domain.h"

HinaPE::SphereDomain::SphereDomain()
{
	NAME = "Domain";
	_switch_surface();
	_switch_bbox();
	flip_normal(); // for inner collision
}
