#include "smoke_solver.h"

HinaPE::SmokeSolver::Data::Data()
{
	_density = std::make_shared<Geom::VertexCenteredScalarGrid3>();
	_density->resize({50, 50, 50}, mVector3::One(), mVector3::Zero(), 0);
	track(&(*_density));

	_domain = std::make_shared<BoxDomain>();
	_domain->POSE.scale = mVector3(25, 25, 25) / 10.0;
}
