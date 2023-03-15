#ifndef HINAPE_GRIDS_BOX_DOMAIN_H
#define HINAPE_GRIDS_BOX_DOMAIN_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/object3D.h"

#include "geom/grid.h"

namespace HinaPE
{
class GridBoxDomain : public Kasumi::ObjectGrid3D
{
public:
	GridBoxDomain()
	{
		NAME = "Domain";
	}
};
} // namespace HinaPE

#endif //HINAPE_GRIDS_BOX_DOMAIN_H
