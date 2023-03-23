#include "smoke_solver.h"

void HinaPE::SmokeSolver::init()
{
	if (_data == nullptr)
		_data = std::make_shared<Data>(mVector3(2, 2, 2), mSize3(64, 64, 64));
	if (_domain == nullptr)
		_domain = std::make_shared<BoxDomain>();
}

void HinaPE::SmokeSolver::update(real dt) const
{
	// accumulate external forces, viscosity force and pressure force
	_accumulate_force();
}

void HinaPE::SmokeSolver::_accumulate_force() const
{
	auto &gravity_y = _data->Fluid.velocity.data_face_v;
	gravity_y.for_each_index(
			[&](size_t i, size_t j, size_t k)
			{
				gravity_y(i, j, k) = -9.8;
			});
}

HinaPE::SmokeSolver::Data::Data(const mVector3 &size, const mSize3 &resolution, const mVector3 &center)
{
	auto spacing = {size.x() / static_cast<real>(resolution.x), size.y() / static_cast<real>(resolution.y), size.z() / static_cast<real>(resolution.z)};
	Fluid.velocity.resize(resolution, spacing, center);
	Fluid.density.resize(resolution, spacing, center);
	Fluid.temperature.resize(resolution, spacing, center);

	Fluid.density.data_center(0, 0, 0) = 1.0;

	track(&Fluid.density);
}
