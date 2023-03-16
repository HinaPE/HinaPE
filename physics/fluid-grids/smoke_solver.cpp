#include "smoke_solver.h"

HinaPE::SmokeSolver::Data::Data()
{
	_density = std::make_shared<Geom::VertexCenteredScalarGrid3>();
	_density->resize({15, 15, 15}, mVector3::One(), mVector3::Zero(), 0);
	track(&(*_density));
}
