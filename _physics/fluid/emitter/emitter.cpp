#include "emitter.h"

void HinaPE::GridEmitter3::update(real current_time, real time_interval)
{
	VALID_CHECK();

	if (_on_begin_update_callback)
		_on_begin_update_callback(current_time, time_interval);
	on_update(current_time, time_interval);
}
void HinaPE::VolumeGridEmitter3::add_signed_distance_target(const HinaPE::Geom::ScalarGrid3Ptr &scalar_grid_target)
{
	ScalarMapper mapper = [](real sdf, const mVector3 &, real old_val) -> real
	{
		return std::min(old_val, sdf);
	};
	add_target(scalar_grid_target, mapper);
}
void HinaPE::VolumeGridEmitter3::add_step_function_target(const HinaPE::Geom::ScalarGrid3Ptr &scalar_grid_target, real min_value, real max_value)
{
	real smoothing_width = scalar_grid_target->_opt.grid_spacing.min();
	ScalarMapper mapper = [min_value, max_value, smoothing_width, scalar_grid_target](real sdf, const mVector3 &, real oldVal)
	{
		real step = Constant::One - Math::smeared_heaviside_sdf(sdf / smoothing_width);
		return std::max(oldVal, (max_value - min_value) * step + min_value);
	};
	add_target(scalar_grid_target, mapper);
}
void HinaPE::VolumeGridEmitter3::add_target(const HinaPE::Geom::ScalarGrid3Ptr &scalar_grid_target, VolumeGridEmitter3::ScalarMapper &mapper) { _scalar_targets.emplace_back(scalar_grid_target, mapper); }
void HinaPE::VolumeGridEmitter3::add_target(const HinaPE::Geom::VectorGrid3Ptr &vector_grid_target, VolumeGridEmitter3::ScalarMapper &mapper) { _vector_targets.emplace_back(vector_grid_target, mapper); }
void HinaPE::VolumeGridEmitter3::on_update(real current_time, real time_interval)
{
	if (!GridEmitter3::_opt.is_enabled)
		return;

	_emit();

	if (_opt.is_one_shot)
		_opt.is_enabled = false;

	_opt.has_emitted = true;
}
void HinaPE::VolumeGridEmitter3::_emit()
{
	_source_region->update_query_engine();

	for (const auto &target: _scalar_targets)
	{
		const auto &grid = std::get<0>(target);
		const auto &mapper = std::get<1>(target);

		auto pos = grid->data_position();
		grid->parallel_for_each_data_point_index(
				[&](size_t i, size_t j, size_t k)
				{
					mVector3 gx = pos(i, j, k);
					real sdf = _source_region->signed_distance(gx);
					(*grid)(i, j, k) = mapper(sdf, gx, (*grid)(i, j, k));
				}
		);
	}
	for (const auto &target: _vector_targets)
	{
		const auto &grid = std::get<0>(target);
		const auto &mapper = std::get<1>(target);

		auto collocated = std::dynamic_pointer_cast<HinaPE::Geom::CollocatedVectorGrid3Ptr>(grid);
		if (collocated != nullptr)
		{
		}
		auto face_centered = std::dynamic_pointer_cast<HinaPE::Geom::FaceCenteredVectorGrid3Ptr>(grid);
		if (face_centered != nullptr)
		{
		}
	}
}
void HinaPE::VolumeGridEmitter3::VALID_CHECK()
{
	if (_scalar_targets.empty() && _vector_targets.empty())
		throw std::runtime_error("VolumeGridEmitter3::_scalar_targets and _vector_targets are empty");
}
