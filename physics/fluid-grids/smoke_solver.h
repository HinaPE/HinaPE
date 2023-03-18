#ifndef HINAPE_SMOKE_SOLVER_H
#define HINAPE_SMOKE_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/object3D.h"

#include "sub_solver/advection_solver.h"
#include "sub_solver/grid_boundary_condition_solver.h"
#include "sub_solver/grid_pressure_solver.h"
#include "sub_solver/grid_diffusion_solver.h"
#include "domain/box_domain.h"
#include "emitter/grid_emitter.h"

namespace HinaPE
{
class SmokeSolver
{
public:
	void init();
	void update(real dt) const;

public:
	struct Data;
	std::shared_ptr<Data> _data;

protected:
	void _accumulate_force() const;
};

struct SmokeSolver::Data : public CopyDisable, public Kasumi::ObjectGrid3D
{
	Geom::FaceCenteredVectorGrid3Ptr _velocity;
	Geom::ScalarGrid3Ptr _density;
	Geom::ScalarGrid3Ptr _temperature;

	mSize3 _resolution = {50, 50, 50};
	mVector3 _origin = mVector3::Zero();
	mVector3 _spacing = mVector3::One();
	bool use_domain_size = false;

	// params
	real _viscosity = 0.0;
	real _max_cfl = 5.0;

	// sub solvers
	AdvectionSolverPtr _advection_solver = std::make_shared<SemiLagrangianSolver>();
	GridBoundaryConditionSolverPtr _boundary_condition_solver = std::make_shared<GridBoundaryConditionSolver>();
	GridPressureSolverPtr _pressure_solver = std::make_shared<GridPressureSolver>();
	GridDiffusionSolverPtr _diffusion_solver = std::make_shared<GridDiffusionSolver>();

	BoxDomainPtr _domain = std::make_shared<BoxDomain>();

	Data();
};
} // namespace HinaPE

#endif //HINAPE_SMOKE_SOLVER_H
