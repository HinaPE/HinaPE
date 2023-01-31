#include "emitter.h"
void Hina::GridEmitter3::update(Hina::real current_time, Hina::real time_interval)
{
	if (_on_begin_update_callback)
		_on_begin_update_callback(current_time, time_interval);
	on_update(current_time, time_interval);
}
void Hina::VolumeGridEmitter3::add_target(const Hina::ScalarGrid3Ptr &scalar_grid_target, Hina::VolumeGridEmitter3::ScalarMapper &mapper) { _scalar_targets.emplace_back(scalar_grid_target, mapper); }
void Hina::VolumeGridEmitter3::add_target(const Hina::VectorGrid3Ptr &vector_grid_target, Hina::VolumeGridEmitter3::ScalarMapper &mapper) { _vector_targets.emplace_back(vector_grid_target, mapper); }
void Hina::VolumeGridEmitter3::on_update(Hina::real current_time, Hina::real time_interval)
{
}