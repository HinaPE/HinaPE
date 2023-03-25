#include "cloud_solver.h"

void CloudSolver::init()
{
	if (_data == nullptr)
		_data = std::make_shared<Data>(mVector3(2, 2, 2), mSize3(200, 200, 200));
	if (_domain == nullptr)
		_domain = std::make_shared<HinaPE::BoxDomain>();

	// init cloud data
	auto &cloud = _data->Cloud.cld;
	auto sphere = std::make_shared<HinaPE::Geom::Sphere3>(0.6);
	auto source_region = std::make_shared<HinaPE::Geom::SurfaceToImplicit3>(sphere);

	auto space = std::min(cloud.spacing.x(), std::min(cloud.spacing.y(), cloud.spacing.z()));
	cloud.data_center.parallel_for_each_index(
			[&](size_t i, size_t j, size_t k)
			{
				mVector3 pt = cloud.pos_center(i, j, k);
				real sdf = source_region->signed_distance(pt);
				real min_value = 0;
				real max_value = 1;
				real step = HinaPE::Constant::One - ((max_value - min_value) * HinaPE::Math::smeared_heaviside_sdf(sdf / cloud.spacing.x()) + min_value);
				cloud.data_center(i, j, k) = step;
			});
}

void CloudSolver::update(real dt) const
{
	_blur_cloud();
	_data->Cloud.cld = _data->Cloud.final;
}

void CloudSolver::reset()
{
	_data->reset();
	init();
}

void CloudSolver::_blur_cloud() const
{
	auto &final = _data->Cloud.final;
	const auto &cloud = _data->Cloud.cld;
	const auto size = cloud.data_center.size();

	int blur_radius = 1;
	cloud.data_center.parallel_for_each_index(
			[&](size_t i, size_t j, size_t k)
			{
				real sum = 0;
				int count = 0;
				for (int di = -blur_radius; di <= blur_radius; ++di)
					for (int dj = -blur_radius; dj <= blur_radius; ++dj)
						for (int dk = -blur_radius; dk <= blur_radius; ++dk)
						{
							int ii = static_cast<int>(i) + di;
							int jj = static_cast<int>(j) + dj;
							int kk = static_cast<int>(k) + dk;
							if (ii < 0 || ii >= size.x || jj < 0 || jj >= size.y || kk < 0 || kk >= size.z)
								continue;
							++count;
							sum += cloud.data_center(ii, jj, kk);
						}
				final.data_center(i, j, k) = sum / count;
			});
}

CloudSolver::Data::Data(const mVector3 &size, const mSize3 &resolution, const mVector3 &center)
{
	auto spacing = {
			size.x() / static_cast<real>(resolution.x),
			size.y() / static_cast<real>(resolution.y),
			size.z() / static_cast<real>(resolution.z)};
	Cloud.hum.resize(resolution, spacing, center);
	Cloud.cld.resize(resolution, spacing, center);
	Cloud.act.resize(resolution, spacing, center);
	Cloud.final.resize(resolution, spacing, center);

	track(&Cloud.final);
}
void CloudSolver::Data::reset()
{
	const auto resolution = Cloud.cld.resolution;
	const auto spacing = Cloud.cld.spacing;
	const auto center = Cloud.cld.center;

	Cloud.hum.clear();
	Cloud.cld.clear();
	Cloud.act.clear();
	Cloud.final.clear();

	Cloud.hum.resize(resolution, spacing, center);
	Cloud.cld.resize(resolution, spacing, center);
	Cloud.act.resize(resolution, spacing, center);
	Cloud.final.resize(resolution, spacing, center);

	track(&Cloud.cld);
}
