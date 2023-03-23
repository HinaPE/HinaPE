#include "smoke_solver.h"

// ============================================================================================================
// ================================================== Solver ==================================================
void HinaPE::SmokeSolver::init()
{
	if (_data == nullptr)
		_data = std::make_shared<Data>(mVector3(2, 2, 2), mSize3(64, 64, 64));
	if (_domain == nullptr)
		_domain = std::make_shared<BoxDomain>();
	if (_emitter == nullptr)
		_emitter = std::make_shared<VolumeGridEmitter3>();

	_emitter->emit(&_data->Fluid.density);
}

void HinaPE::SmokeSolver::update(real dt) const
{
	_accumulate_force();


}

void HinaPE::SmokeSolver::reset()
{
	_data->reset();
	init();
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

void HinaPE::SmokeSolver::_compute_advection() const
{
}

void HinaPE::SmokeSolver::INSPECT()
{
}
// ================================================== Solver ==================================================
// ============================================================================================================

// ==========================================================================================================
// ================================================== Data ==================================================
HinaPE::SmokeSolver::Data::Data(const mVector3 &size, const mSize3 &resolution, const mVector3 &center)
{
	auto spacing = {
			size.x() / static_cast<real>(resolution.x),
			size.y() / static_cast<real>(resolution.y),
			size.z() / static_cast<real>(resolution.z)};
	Fluid.velocity.resize(resolution, spacing, center);
	Fluid.density.resize(resolution, spacing, center);
	Fluid.temperature.resize(resolution, spacing, center);

	track(&Fluid.density);
}
void HinaPE::SmokeSolver::Data::reset()
{
}
// ================================================== Data ==================================================
// ==========================================================================================================