#include "point_neighbor_search.h"

// ==================== PointSimpleListSearch3 ====================
void HinaPE::PointSimpleListSearch3::for_each_nearby_point(const mVector3 &origin, const HinaPE::PointNeighborSearch3::ForEachNearbyPointFunc &callback)
{
	real radius_squared = _radius * _radius;

	for (int i = 0; i < _points.size(); ++i)
	{
		mVector3 r = _points[i] - origin;
		real distance_squared = r.dot(r);
		if (distance_squared <= radius_squared)
			callback(i, _points[i]);
	}
}

auto HinaPE::PointSimpleListSearch3::has_nearby_point(const mVector3 &origin) const -> bool
{
	real radius_squared = _radius * _radius;

	return std::any_of(_points.begin(), _points.end(), [radius_squared, origin](const mVector3 &point)
	{
		mVector3 r = point - origin;
		real distance_squared = r.dot(r);
		if (distance_squared <= radius_squared)
			return true;
		return false;
	});
}

void HinaPE::PointSimpleListSearch3::build(const std::vector<mVector3> &points)
{
	_points.resize(points.size());
	std::copy(points.begin(), points.end(), _points.begin());
}


// ==================== PointHashGridSearch3 ====================
void HinaPE::PointHashGridSearch3::for_each_nearby_point(const mVector3 &origin, const HinaPE::PointNeighborSearch3::ForEachNearbyPointFunc &callback)
{
	if (_buckets.empty())
		return;

	auto nearby_keys = _get_nearby_keys(origin);
	const real query_radius_squared = _radius * _radius;

	for (auto key: nearby_keys)
	{
		for (auto i: _buckets[key])
		{
			mVector3 r = _points[i] - origin;
			real distance_squared = r.dot(r);
			if (distance_squared <= query_radius_squared)
				callback(i, _points[i]);
		}
	}
}

auto HinaPE::PointHashGridSearch3::has_nearby_point(const mVector3 &origin) const -> bool
{
	if (_buckets.empty())
		return false;

	auto nearby_keys = _get_nearby_keys(origin);
	const real query_radius_squared = _radius * _radius;

	for (auto key: nearby_keys)
	{
		for (auto i: _buckets[key])
		{
			mVector3 r = _points[i] - origin;
			real distance_squared = r.dot(r);
			if (distance_squared <= query_radius_squared)
				return true;
		}
	}

	return false;
}

void HinaPE::PointHashGridSearch3::build(const std::vector<mVector3> &points)
{
	_buckets.clear();
	_points.clear();

	_buckets.resize(_resolution.x * _resolution.y * _resolution.z);
	_points.resize(points.size());

	if (points.empty()) return;

	for (size_t i = 0; i < points.size(); ++i)
	{
		_points[i] = points[i];
		size_t key = _get_hash_key_from_position(points[i]);
		_buckets[key].push_back(i);
	}
}
void HinaPE::PointHashGridSearch3::add_point(const mVector3 &point)
{
	if (_buckets.empty())
	{
		std::vector<mVector3> points;
		points.push_back(point);
		build(points);
	} else
	{
		size_t i = _points.size();
		_points.push_back(point);
		size_t key = _get_hash_key_from_position(point);
		_buckets[key].push_back(i);
	}
}

auto HinaPE::PointHashGridSearch3::_get_hash_key_from_position(const mVector3 &position) const -> size_t { return _get_hash_key_from_bucket_index(_get_bucket_index(position)); }
auto HinaPE::PointHashGridSearch3::_get_nearby_keys(const mVector3 &position) const -> std::array<size_t, 8>
{
	auto origin_index = _get_bucket_index(position);
	std::array<mVector3i, 8> nearby_bucket_indices;

	for (int i = 0; i < 8; ++i)
		nearby_bucket_indices[i] = origin_index;

	if ((static_cast<real>(origin_index.x()) + HinaPE::Constant::Half) * _grid_spacing <= position.x())
	{
		nearby_bucket_indices[4].x() += 1;
		nearby_bucket_indices[5].x() += 1;
		nearby_bucket_indices[6].x() += 1;
		nearby_bucket_indices[7].x() += 1;
	} else
	{
		nearby_bucket_indices[4].x() -= 1;
		nearby_bucket_indices[5].x() -= 1;
		nearby_bucket_indices[6].x() -= 1;
		nearby_bucket_indices[7].x() -= 1;
	}

	if ((static_cast<real>(origin_index.y()) + HinaPE::Constant::Half) * _grid_spacing <= position.y())
	{
		nearby_bucket_indices[2].y() += 1;
		nearby_bucket_indices[3].y() += 1;
		nearby_bucket_indices[6].y() += 1;
		nearby_bucket_indices[7].y() += 1;
	} else
	{
		nearby_bucket_indices[2].y() -= 1;
		nearby_bucket_indices[3].y() -= 1;
		nearby_bucket_indices[6].y() -= 1;
		nearby_bucket_indices[7].y() -= 1;
	}

	if ((static_cast<real>(origin_index.z()) + HinaPE::Constant::Half) * _grid_spacing <= position.z())
	{
		nearby_bucket_indices[1].z() += 1;
		nearby_bucket_indices[3].z() += 1;
		nearby_bucket_indices[5].z() += 1;
		nearby_bucket_indices[7].z() += 1;
	} else
	{
		nearby_bucket_indices[1].z() -= 1;
		nearby_bucket_indices[3].z() -= 1;
		nearby_bucket_indices[5].z() -= 1;
		nearby_bucket_indices[7].z() -= 1;
	}

	std::array<size_t, 8> res = {};
	for (int i = 0; i < 8; ++i)
		res[i] = _get_hash_key_from_bucket_index(nearby_bucket_indices[i]);

	return res;
}
auto HinaPE::PointHashGridSearch3::_get_hash_key_from_bucket_index(const mVector3i &index) const -> size_t
{
	mVector3i wrapped_index = index;

	wrapped_index.x() = index.x() % static_cast<int>(_resolution.x);
	wrapped_index.y() = index.y() % static_cast<int>(_resolution.y);
	wrapped_index.z() = index.z() % static_cast<int>(_resolution.z);

	if (wrapped_index.x() < 0) wrapped_index.x() += static_cast<int>(_resolution.x);
	if (wrapped_index.y() < 0) wrapped_index.y() += static_cast<int>(_resolution.y);
	if (wrapped_index.z() < 0) wrapped_index.z() += static_cast<int>(_resolution.z);

	return static_cast<size_t>(wrapped_index.z() * _resolution.y + wrapped_index.y()) * _resolution.x + wrapped_index.x();
}
auto HinaPE::PointHashGridSearch3::_get_bucket_index(const mVector3 &position) const -> mVector3i
{
	mVector3i res;
	res.x() = static_cast<int>(std::floor(position.x() / _grid_spacing));
	res.y() = static_cast<int>(std::floor(position.y() / _grid_spacing));
	res.z() = static_cast<int>(std::floor(position.z() / _grid_spacing));
	return res;
}

