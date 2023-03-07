#include "box_domain.h"

HinaPE::BoxDomain::BoxDomain() : Kasumi::CubeObject()
{
	NAME = "Domain";
	_switch_surface();
	_switch_bbox();
	flip_normal(); // for inner collision

	POSE.scale = {3.0, 3.0, 3.0};
}
