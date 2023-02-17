#ifndef HINAPE_SOLVER_H
#define HINAPE_SOLVER_H

#include "common.h"
#include "neighbor_search/point_simple_list_searcher.h"

namespace HinaPE
{
class SPHSolver : public CopyDisable
{
public:
	void step(real dt);

public:
	struct Opt
	{
	} _opt;
	SPHSolver();

protected:

public:
	struct Data;
	std::shared_ptr<Data> _data;

	real _current_dt;
};

struct SPHSolver::Data : public CopyDisable
{
public:
	struct Opt
	{
		// common
		real mass = 1e-3;
		real radius = 1e-3;
		real max_search_radius = 1e-3;

		// sph
		real target_density = 1000; // water density
		real target_spacing = 0.1;
		real kernel_radius_over_target_spacing = 1.8;
		real kernel_radius = kernel_radius_over_target_spacing * target_spacing;
	} _opt;
	void _rebuild_();

	// particles
	std::vector<mVector3> _positions;
	std::vector<mVector3> _velocities;
	std::vector<mVector3> _forces;
	std::vector<real> _densities;
	std::vector<real> _pressures;

	// neighbor searcher
	PointNeighborSearcher3Ptr _neighbor_searcher;
	std::vector<std::vector<size_t>> _neighbor_lists;
};
} // namespace HinaPE

#endif //HINAPE_SOLVER_H
