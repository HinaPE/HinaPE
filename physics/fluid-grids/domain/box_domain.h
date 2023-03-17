#ifndef HINAPE_GRIDS_BOX_DOMAIN_H
#define HINAPE_GRIDS_BOX_DOMAIN_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/cube.h"

namespace HinaPE
{
class BoxDomain : public Kasumi::CubeObject
{
public:
	BoxDomain()
	{
		NAME = "Domain";
		_switch_surface();
		_switch_bbox();
		flip_normal(); // for inner collision
		set_color(HinaPE::Color::WHITE);
	}
};
using BoxDomainPtr = std::shared_ptr<BoxDomain>;
} // namespace HinaPE

#endif //HINAPE_GRIDS_BOX_DOMAIN_H
