#include "smoke_solver.h"

HinaPE::SmokeSolver::Data::Data()
{
	_density = std::make_shared<Geom::VertexCenteredScalarGrid3>();
	_density->resize({50, 50, 50}, mVector3::One(), mVector3::Zero(), 0);
	(*_density)(1,2,3) = 1;
	(*_density)(11,22,33) = 1;
	track(&(*_density));
}
