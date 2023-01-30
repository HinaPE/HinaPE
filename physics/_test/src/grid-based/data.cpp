#include "data.h"

Hina::GridSystemData3::GridSystemData3(const Hina::GridSystemData3::Opt &opt)
{
	_velocity = std::make_shared<FaceCenteredVectorGrid3>();
	_velocity_index = _advectable_vector_data.size();
	_advectable_vector_data.push_back(_velocity);
	resize(opt.resolution, opt.grid_spacing, opt.origin);
}
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
