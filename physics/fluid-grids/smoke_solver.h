#ifndef HINAPE_SMOKE_SOLVER_H
#define HINAPE_SMOKE_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/object3D.h"

#include "sub_solver/advection_solver.h"
#include "domain/box_domain.h"

namespace HinaPE
{
class SmokeSolver
{
public:
	struct Data;
	std::shared_ptr<Data> _data;
};

struct SmokeSolver::Data : public CopyDisable, public Kasumi::ObjectGrid3D
{
	Geom::FaceCenteredVectorGrid3Ptr _velocity;
	Geom::ScalarGrid3Ptr _density;
	Geom::ScalarGrid3Ptr _temperature;

	mSize3 _resolution;
	mVector3 _origin;
	mVector3 _spacing;
	real domain_size = 1.0;
	bool use_domain_size = false;

	// params
	real _viscosity = 0.0;
	real _max_cfl = 5.0;

	// sub solvers
	AdvectionSolverPtr _advection_solver;

	Data();
};
} // namespace HinaPE

#endif //HINAPE_SMOKE_SOLVER_H
