#ifndef HINAPE_PCISPH_SOLVER_H
#define HINAPE_PCISPH_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"
#include "neighbor_search/compact_nsearch.h"

// @formatter:off
namespace HinaPE
{
class PCISPHSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
	void update(real dt) const;

public:
	struct Opt
	{} _opt;
	struct Data;
	std::shared_ptr<Data> 	_data;
	BoxDomainPtr 			_domain;
	ParticleEmitter3Ptr 	_emitter;

protected:
	void INSPECT() final;
	void VALID_CHECK() const final;
};

struct PCISPHSolver::Data : public CopyDisable, public Kasumi::ObjectParticles3D
{
public:
	// particles
	std::vector<mVector3> 	_positions;
	std::vector<mVector3> 	_velocities;
	std::vector<mVector3> 	_forces;
	std::vector<real> 		_densities;
	std::vector<real> 		_pressures;

protected:
	friend class PCISPHSolver;
	void _update_poses() final;
};
} // namespace HinaPE

#endif //HINAPE_PCISPH_SOLVER_H
