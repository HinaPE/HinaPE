#ifndef HINAPE_SMOKE_SOLVER_H
#define HINAPE_SMOKE_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "backends/objects/object3D.h"
#include "domain/box_domain.h"

//#include "sub_solver/advection_solver.h"
//#include "sub_solver/grid_boundary_condition_solver.h"
//#include "sub_solver/grid_pressure_solver.h"
//#include "sub_solver/grid_diffusion_solver.h"
//#include "domain/box_domain.h"
//#include "emitter/grid_emitter.h"
//
namespace HinaPE
{
class SmokeSolver
{
protected:
	void _accumulate_force() const;

public:
	void init();
	void update(real dt) const;

public:
	struct Data;
	std::shared_ptr<Data> _data;
	std::shared_ptr<BoxDomain> _domain;
};

struct SmokeSolver::Data : public CopyDisable, public Kasumi::ObjectGrid3D
{
	struct
	{
		Geom::ValuedGrid3<real> velocity;
		Geom::ValuedGrid3<real> density;
		Geom::ValuedGrid3<real> temperature;
	} Fluid;

	Data(const mVector3& size, const mSize3 &resolution, const mVector3& center = mVector3::Zero());
};
} // namespace HinaPE

#endif //HINAPE_SMOKE_SOLVER_H
