#include "smoke_solver.h"

void HinaPE::SmokeSolver::init()
{
}

void HinaPE::SmokeSolver::update(real dt) const
{
	// accumulate external forces, viscosity force and pressure force
	_accumulate_force();
}

void HinaPE::SmokeSolver::_accumulate_force() const
{
	auto &vel = _data->_velocity;
	vel->for_each_v_index(
			[&](size_t i, size_t j, size_t k)
			{
				vel->v(i, j, k) = -9.8;
			});
}

HinaPE::SmokeSolver::Data::Data()
{
	_density = std::make_shared<Geom::VertexCenteredScalarGrid3>();
	_density->resize(_resolution, _spacing, _origin, 0);
	track(&(*_density));

	_velocity = std::make_shared<Geom::FaceCenteredVectorGrid3>();
	_velocity->resize(_resolution, _spacing, _origin, mVector3::Zero());

	_domain->POSE.scale = 0.1 * mVector3(25, 25, 25);
}
