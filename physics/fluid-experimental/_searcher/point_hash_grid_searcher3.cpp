#include "point_hash_grid_searcher3.h"
auto HinaPE::PointHashGridSearcher3::type_name() const -> std::string { return "PointHashGridSearcher3"; }
void HinaPE::PointHashGridSearcher3::build(const HinaPE::ConstArrayAccessor1<mVector3> &points)
{
	_buckets.clear();
	_points.clear();

	_buckets.resize(_resolution.x * _resolution.y * _resolution.z);
	_points.resize(points.size());

	if (points.size() == 0)
		return;

	for (int i = 0; i < points.size(); ++i)
	{
		_points[i] = points[i];
		size_t key = get_hash_key_from_position(points[i]);
		_buckets[key].push_back(i);
	}
}
void HinaPE::PointHashGridSearcher3::for_each_nearby_point(const mVector3 &origin, real radius, const std::function<void(size_t, const mVector3 &)> &callback) const
{
	if (_buckets.empty())
		return;

	std::array<size_t, 8> nearby_keys = get_nearby_keys(origin);
	const real radius_squared = radius * radius;
	for (int i = 0; i < 8; ++i)
	{
		const auto &bucket = _buckets[nearby_keys[i]];

		for (unsigned long long point_index: bucket)
		{
			real r_squared = (_points[point_index] - origin).lengthSquared();
			if (r_squared <= radius_squared)
				callback(point_index, _points[point_index]);
		}
	}
}
auto HinaPE::PointHashGridSearcher3::has_nearby_point(const mVector3 &origin, real radius) const -> bool
{
	if (_buckets.empty())
		return false;

	std::array<size_t, 8> nearby_keys = get_nearby_keys(origin);
	const real radius_squared = radius * radius;

	for (int i = 0; i < 8; ++i)
	{
		const auto &bucket = _buckets[nearby_keys[i]];

		for (unsigned long long point_index: bucket)
		{
			real r_squared = (_points[point_index] - origin).lengthSquared();
			if (r_squared <= radius_squared)
				return true;
		}
	}

	return false;
}
auto HinaPE::PointHashGridSearcher3::clone() const -> std::shared_ptr<PointNeighborSearcher3>
{
	return std::shared_ptr<PointHashGridSearcher3>(
			new PointHashGridSearcher3(*this),
			[](PointHashGridSearcher3 *p) { delete p; }
	);
}
auto HinaPE::PointHashGridSearcher3::get_hash_key_from_position(const mVector3 &position) -> size_t
{
	mPoint3I bucket_index = get_bucket_index(position);
	return get_hash_key_from_bucket_index(bucket_index);
}
auto HinaPE::PointHashGridSearcher3::get_bucket_index(const mVector3 &position) const -> mPoint3I
{
	mPoint3I bucket_index;
	bucket_index.x = static_cast<int>(std::floor(position.x / _grid_spacing));
	bucket_index.y = static_cast<int>(std::floor(position.y / _grid_spacing));
	bucket_index.z = static_cast<int>(std::floor(position.z / _grid_spacing));
	return bucket_index;
}
auto HinaPE::PointHashGridSearcher3::get_hash_key_from_bucket_index(const mPoint3I &bucket_index) const -> size_t
{
	mPoint3I wrapped_index = bucket_index;
	wrapped_index.x = bucket_index.x % _resolution.x;
	wrapped_index.y = bucket_index.y % _resolution.y;
	wrapped_index.z = bucket_index.z % _resolution.z;
	if (wrapped_index.x < 0)
		wrapped_index.x += _resolution.x;
	if (wrapped_index.y < 0)
		wrapped_index.y += _resolution.y;
	if (wrapped_index.z < 0)
		wrapped_index.z += _resolution.z;
	return static_cast<size_t>(wrapped_index.x + _resolution.x * (wrapped_index.y + _resolution.y * wrapped_index.z));
}
auto HinaPE::PointHashGridSearcher3::get_nearby_keys(const mVector3 &position) const -> std::array<size_t, 8>
{
	std::array<size_t, 8> res{};
	mPoint3I origin_index = get_bucket_index(position);

	std::array<mPoint3I, 8> nearby_bucket_indices{};
	for (auto &nearby_bucket_index: nearby_bucket_indices)
		nearby_bucket_index = origin_index;

	if ((static_cast<real>(origin_index.x) + 0.5f) * _grid_spacing < position.x)
	{
		nearby_bucket_indices[4].x += 1;
		nearby_bucket_indices[5].x += 1;
		nearby_bucket_indices[6].x += 1;
		nearby_bucket_indices[7].x += 1;
	} else
	{
		nearby_bucket_indices[4].x -= 1;
		nearby_bucket_indices[5].x -= 1;
		nearby_bucket_indices[6].x -= 1;
		nearby_bucket_indices[7].x -= 1;
	}

	if ((static_cast<real>(origin_index.y) + 0.5f) * _grid_spacing < position.y)
	{
		nearby_bucket_indices[2].y += 1;
		nearby_bucket_indices[3].y += 1;
		nearby_bucket_indices[6].y += 1;
		nearby_bucket_indices[7].y += 1;
	} else
	{
		nearby_bucket_indices[2].y -= 1;
		nearby_bucket_indices[3].y -= 1;
		nearby_bucket_indices[6].y -= 1;
		nearby_bucket_indices[7].y -= 1;
	}

	if ((static_cast<real>(origin_index.z) + 0.5f) * _grid_spacing < position.z)
	{
		nearby_bucket_indices[1].z += 1;
		nearby_bucket_indices[3].z += 1;
		nearby_bucket_indices[5].z += 1;
		nearby_bucket_indices[7].z += 1;
	} else
	{
		nearby_bucket_indices[1].z -= 1;
		nearby_bucket_indices[3].z -= 1;
		nearby_bucket_indices[5].z -= 1;
		nearby_bucket_indices[7].z -= 1;
	}

	for (int i = 0; i < 8; ++i)
		res[i] = get_hash_key_from_bucket_index(nearby_bucket_indices[i]);

	return res;
}
void HinaPE::PointHashGridSearcher3::add(const mVector3 &point)
{
	if (_buckets.empty())
	{
		Array1<mVector3> arr = {point};
		build(arr);
	} else
	{
		size_t i = _points.size();
		_points.push_back(point);
		size_t key = get_hash_key_from_position(point);
		_buckets[key].push_back(i);
	}
}
