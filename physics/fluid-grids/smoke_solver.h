#ifndef HINAPE_SMOKE_SOLVER_H
#define HINAPE_SMOKE_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/object3D.h"
#include "domain/box_domain.h"
#include "emitter/grid_emitter.h"

#include "sub_solver/advection_solver.h"
#include "sub_solver/grid_boundary_condition_solver.h"
#include "sub_solver/grid_pressure_solver.h"
#include "sub_solver/grid_diffusion_solver.h"

// @formatter:off
namespace HinaPE
{
class SmokeSolver : public Kasumi::INSPECTOR
{
protected:
	void _accumulate_force() const;
	void _compute_advection() const;

public:
	void init();
	void update(real dt) const;
	void reset();

	struct Data;
	std::shared_ptr<Data> 				_data;
	std::shared_ptr<BoxDomain> 			_domain;
	std::shared_ptr<VolumeGridEmitter3> _emitter;

	struct
	{
		real current_dt = 0.02;
	} Opt;

private:
	void INSPECT() final;
};

struct SmokeSolver::Data : public CopyDisable, public Kasumi::ObjectGrid3D
{
	struct
	{
		Geom::ScalarGridField3 velocity;
		Geom::ScalarGridField3 density;
		Geom::ScalarGridField3 temperature;
	} Fluid;

	Data(const mVector3 &size, const mSize3 &resolution, const mVector3 &center = mVector3::Zero());
	void reset();
};
} // namespace HinaPE

#endif //HINAPE_SMOKE_SOLVER_H
