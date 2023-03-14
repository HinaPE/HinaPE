#ifndef HINAPE_PARTICLES_SPHERE_DOMAIN_H
#define HINAPE_PARTICLES_SPHERE_DOMAIN_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/sphere.h"

namespace HinaPE
{
class SphereDomain : public Kasumi::SphereObject
{
public:
	SphereDomain()
	{
		NAME = "Domain";
		_switch_surface();
		_switch_bbox();
		flip_normal(); // for inner collision
	}
};
} // namespace HinaPE

#endif //HINAPE_PARTICLES_SPHERE_DOMAIN_H
