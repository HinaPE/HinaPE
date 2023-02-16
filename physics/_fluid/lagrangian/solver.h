#ifndef HINAPE_SOLVER_H
#define HINAPE_SOLVER_H

#include "common.h"

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
		real width = 7.0f;
		real height = 5.0f;
		int rows = 30;
		int cols = 30;
	} _opt;
	void _sync_opt();

	// init infos

	// update infos
};
} // namespace HinaPE

#endif //HINAPE_SOLVER_H
