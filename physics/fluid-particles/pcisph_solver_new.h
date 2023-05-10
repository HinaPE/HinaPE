#ifndef HINAPE_PCISPH_SOLVER_NEW_H
#define HINAPE_PCISPH_SOLVER_NEW_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"
#include "domain/sphere_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"

namespace HinaPE
{
class PCISPHSolverNew : public Kasumi::INSPECTOR
{
protected:
	void _update_neighbor() const;
	void _update_density() const;

public:
	struct
	{
	} Opt;
	struct Data;
	void init();
	void update(real dt) const;
	void reset();

protected:
	void INSPECT() override;
};

struct PCISPHSolverNew::Data : public Kasumi::ObjectParticles3D
{
	struct // fluid particles
	{
		std::vector<mVector3> 	positions;
		std::vector<mVector3> 	velocities;
		std::vector<mVector3> 	forces;
		std::vector<real> 		densities;
		std::vector<real> 		pressures;
		std::vector<real> 		masses;
	} Fluid;

	std::vector<std::vector<unsigned int>> 	NeighborList;

	explicit Data();
};
} // namespace HinaPE

#endif //HINAPE_PCISPH_SOLVER_NEW_H
