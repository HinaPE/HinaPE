#include "grid.h"

void Hina::Grid3::resize(const Hina::Base::Size3 &resolution, const Hina::mVector3 &grid_spacing, const Hina::mVector3 &origin)
{
	_opt.resolution = resolution;
	_opt.grid_spacing = grid_spacing;
	_opt.origin = origin;
	_opt.bounding_box = Hina::mBBox3(
			origin,
			origin + grid_spacing * Hina::mVector3(
					static_cast<real>(resolution.x),
					static_cast<real>(resolution.y),
					static_cast<real>(resolution.z)));
}
auto Hina::Grid3::cell_center_position() const -> Hina::Grid3::DataPositionFunc
{
	mVector3 h = _opt.grid_spacing;
	mVector3 o = _opt.origin;
	return [h, o](size_t i, size_t j, size_t k) { return o + h * mVector3(static_cast<real>(i) + 0.5, static_cast<real>(j) + 0.5, static_cast<real>(k) + 0.5); };
}
void Hina::Grid3::for_each_cell_index(const std::function<void(size_t, size_t, size_t)> &func) const
{
	for (size_t k = 0; k < _opt.resolution.z; ++k)
		for (size_t j = 0; j < _opt.resolution.y; ++j)
			for (size_t i = 0; i < _opt.resolution.x; ++i)
				func(i, j, k);
}
void Hina::Grid3::parallel_for_each_cell_index(const std::function<void(size_t, size_t, size_t)> &func) const
{
	Base::parallelFor((size_t) 0, _opt.resolution.x, (size_t) 0, _opt.resolution.y, (size_t) 0, _opt.resolution.z, [&func](size_t i, size_t j, size_t k) { func(i, j, k); }); // TODO: why another lambda
}




void Hina::ScalarGrid3::resize(const Hina::Base::Size3 &resolution, const Hina::mVector3 &grid_spacing, const Hina::mVector3 &origin, Hina::real initial_value)
{
	// update opt
	_opt.resolution = resolution;
	_opt.grid_spacing = grid_spacing;
	_opt.origin = origin;
	_opt.bounding_box = mBBox3(origin, origin + grid_spacing * mVector3(static_cast<real>(resolution.x), static_cast<real>(resolution.y), static_cast<real>(resolution.z)));

	// update data
	Grid3::resize(resolution, grid_spacing, origin);
	_data.resize(data_size(), initial_value);
}




void Hina::VectorGrid3::resize(const Hina::Base::Size3 &resolution, const Hina::mVector3 &grid_spacing, const Hina::mVector3 &origin, const Hina::mVector3 &initial_value)
{
	Grid3::resize(resolution, grid_spacing, origin);
	on_resize(resolution, grid_spacing, origin, initial_value);
}
void Hina::CollocatedVectorGrid3::on_resize(const Hina::Base::Size3 &resolution, const Hina::mVector3 &grid_spacing, const Hina::mVector3 &origin, const Hina::mVector3 &initial_value)
{
}
void Hina::FaceCenteredVectorGrid3::on_resize(const Hina::Base::Size3 &resolution, const Hina::mVector3 &grid_spacing, const Hina::mVector3 &origin, const Hina::mVector3 &initial_value)
{
}
