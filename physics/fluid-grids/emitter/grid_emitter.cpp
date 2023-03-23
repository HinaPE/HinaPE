#include "grid_emitter.h"

HinaPE::VolumeGridEmitter3::VolumeGridEmitter3()
{
	auto sphere = std::make_shared<Geom::Sphere3>(0.3);
	_source_region = std::make_shared<Geom::SurfaceToImplicit3>(sphere);
}
void HinaPE::VolumeGridEmitter3::emit(HinaPE::Geom::DataGrid3<real> *grid)
{
	if (_source_region == nullptr)
		throw std::runtime_error("source region is null");

	auto space = std::min(grid->spacing.x(), std::min(grid->spacing.y(), grid->spacing.z()));

	grid->data_center.parallel_for_each_index(
			[&](size_t i, size_t j, size_t k)
			{
				mVector3 pt = grid->pos_center(i, j, k);
				real sdf = _source_region->signed_distance(pt);

				real old = grid->data_center(i, j, k);
				real min_value = 0;
				real max_value = 1;
				real step = Constant::One - ((max_value - min_value) * Math::smeared_heaviside_sdf(sdf / space) + min_value);
				grid->data_center(i, j, k) = std::max(old, step);
			});
}
