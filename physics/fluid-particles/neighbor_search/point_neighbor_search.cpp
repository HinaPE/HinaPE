#include "point_neighbor_search.h"

void HinaPE::PointSimpleListSearch3::for_each_nearby_point(const mVector3 &origin, real radius, const HinaPE::PointNeighborSearch3::ForEachNearbyPointFunc &callback)
{
	real radius_squared = radius * radius;

	for (int i = 0; i < _points.size(); ++i)
	{
		mVector3 r = _points[i] - origin;
		real distance_squared = r.dot(r);
		if (distance_squared <= radius_squared)
			callback(i, _points[i]);
	}
}
auto HinaPE::PointSimpleListSearch3::has_nearby_point(const mVector3 &origin, real radius) const -> bool
{
	real radius_squared = radius * radius;

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
	std::copy(points.data(), points.data() + points.size(), _points.begin());
}

void HinaPE::PointHashGridSearch3::for_each_nearby_point(const mVector3 &origin, real radius, const HinaPE::PointNeighborSearch3::ForEachNearbyPointFunc &callback)
{
}
auto HinaPE::PointHashGridSearch3::has_nearby_point(const mVector3 &origin, real radius) const -> bool
{
	return false;
}
void HinaPE::PointHashGridSearch3::build(const std::vector<mVector3> &points)
{
}

