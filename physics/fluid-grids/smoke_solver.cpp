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
	_compute_advection();
}

void HinaPE::SmokeSolver::reset()
{
	_data->reset();
	init();
}

void HinaPE::SmokeSolver::_accumulate_force() const
{
	auto &v_y = _data->Fluid.velocity.data_face_v;
	v_y.for_each_index(
			[&](size_t i, size_t j, size_t k)
			{
				v_y(i, j, k) += Opt.current_dt * -9.8;
			});
}

void HinaPE::SmokeSolver::_compute_advection() const
{
	const auto &flow = _data->Fluid.velocity;
	const auto dt = Opt.current_dt;

	// advect density
	auto &d_output = _data->Fluid.density;
	auto d_input = _data->Fluid.density; // copy

	const real h = d_output.spacing.min();
	d_output.data_center.parallel_for_each_index(
			[&](size_t i, size_t j, size_t k)
			{
				const mVector3 &pt0 = d_output.pos_center(i, j, k);
				const mVector3 &vel0 = flow.sample_uvw(pt0);

				mVector3 mid_pt = pt0 - 0.5 * dt * vel0;
				mVector3 mid_vel = flow.sample_uvw(mid_pt);

				mVector3 pt1 = pt0 - dt * mid_vel;

				d_output.data_center(i, j, k) = d_input.sample(pt1);
			});
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