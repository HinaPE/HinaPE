#ifndef HINAPE_SMOKE_SOLVER_H
#define HINAPE_SMOKE_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/object3D.h"
#include "geom/grid.h"

namespace HinaPE
{
class SmokeSolver
{
public:
	struct Data;
};

class SmokeSolver::Data : public CopyDisable, public Kasumi::ObjectGrid3D
{
public:
	Geom::FaceCenteredVectorGrid3Ptr _velocity;
	Geom::ScalarGrid3Ptr _density;
	Geom::ScalarGrid3Ptr _temperature;

	Data();
};
} // namespace HinaPE

#endif //HINAPE_SMOKE_SOLVER_H
