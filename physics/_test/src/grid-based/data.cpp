#include "data.h"

void Hina::GridSystemData3::resize(const Hina::Base::Size3 &resolution, const Hina::Base::mVector3 &grid_spacing, const Hina::Base::mVector3 &origin)
{
	_opt.resolution = resolution;
	_opt.grid_spacing = grid_spacing;
	_opt.origin = origin;

	for (auto &data: _scalar_data)
		data->resize(resolution, grid_spacing, origin, Base::Zero);
	for (auto &data: _vector_data)
		data->resize(resolution, grid_spacing, origin, Base::mVector3::Zero());
	for (auto &data: _advectable_scalar_data)
		data->resize(resolution, grid_spacing, origin, Base::Zero);
	for (auto &data: _advectable_vector_data)
		data->resize(resolution, grid_spacing, origin, Base::mVector3::Zero());
}
