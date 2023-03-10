#ifndef HINAPE_PCISPH_SOLVER_H
#define HINAPE_PCISPH_SOLVER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "domain/box_domain.h"
#include "kernel/sph_kernel.h"
#include "emitter/particle_emitter.h"
#include "neighbor_search/point_neighbor_search.h"
#include "neighbor_search/compact_nsearch.h"

// @formatter:on
namespace HinaPE
{
class PCISPHSolver : public CopyDisable, public Kasumi::INSPECTOR, public Kasumi::VALID_CHECKER
{
public:
	void update(real dt) const;

public:
	struct Opt
	{
		bool inited = false;
		real current_dt = 0.02;
		mVector3 gravity = mVector3(0, -9.8, 0);
		real restitution = 0.3;
	} _opt;
	struct Data;
	std::shared_ptr<Data> _data;
	BoxDomainPtr _domain;
	ParticleEmitter3Ptr _emitter;

protected:
	void _emit_particles() const;
	void _accumulate_force() const;
	void _time_integration() const;
	void _resolve_collision() const;

	void INSPECT() final;
	void VALID_CHECK() const final;
};

struct PCISPHSolver::Data : public CopyDisable, public Kasumi::ObjectParticles3D
{
public:
	// particles
	std::vector<mVector3> _positions;
	std::vector<mVector3> _velocities;
	std::vector<mVector3> _forces;
	std::vector<real> _densities;
	std::vector<real> _pressures;

protected:
	friend class PCISPHSolver;
};
} // namespace HinaPE

#endif //HINAPE_PCISPH_SOLVER_H
