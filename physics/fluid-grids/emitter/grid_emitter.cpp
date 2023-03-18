#include "grid_emitter.h"

HinaPE::VolumeGridEmitter3::VolumeGridEmitter3()
{
	auto sphere = std::make_shared<Geom::Sphere3>();
	sphere->_transform._translation = mVector3(25, 25, 25);
	_source_region = std::make_shared<Geom::SurfaceToImplicit3>(sphere);
}

void HinaPE::VolumeGridEmitter3::emit(Geom::ScalarGrid3 *grid)
{
	_grid_spacing = std::min(grid->_opt.grid_spacing.x(), std::min(grid->_opt.grid_spacing.y(), grid->_opt.grid_spacing.z()));
	auto pos = grid->data_position();
	grid->parallel_for_each_data_point_index(
			[&](size_t i, size_t j, size_t k)
			{
				mVector3 pt = pos(i, j, k);
				real sdf = _source_region->signed_distance(pt);
				real old = (*grid)(i, j, k);
				(*grid)(i, j, k) = scalar_mapper(sdf, pt, old);
			});
}
