#include "point_simple_list_searcher3.h"
auto HinaPE::PointSimpleListSearcher3::type_name() const -> std::string
{
	return "PointSimpleListSearcher3";
}
void HinaPE::PointSimpleListSearcher3::build(const HinaPE::ConstArrayAccessor1<mVector3> &points)
{
	_points.resize(points.size());
	std::copy(points.data(), points.data() + points.size(), _points.begin());
}
void HinaPE::PointSimpleListSearcher3::for_each_nearby_point(const mVector3 &origin, real radius, const std::function<void(size_t, const mVector3 &)> &callback) const
{
	real radius_spared = radius * radius;
	for (size_t i = 0; i < _points.size(); ++i)
	{
		mVector3 r = _points[i] - origin;
		real distance_spared = r.dot(r);
		if (distance_spared <= radius_spared)
			callback(i, _points[i]);
	}
}
auto HinaPE::PointSimpleListSearcher3::has_nearby_point(const mVector3 &origin, real radius) const -> bool
{
	real radius_spared = radius * radius;
	for (const auto &_point: _points)
	{
		mVector3 r = _point - origin;
		real distance_spared = r.dot(r);
		if (distance_spared <= radius_spared)
			return true;
	}
	return false;
}
auto HinaPE::PointSimpleListSearcher3::clone() const -> std::shared_ptr<PointNeighborSearcher3>
{
	return std::shared_ptr<PointSimpleListSearcher3>(
			new PointSimpleListSearcher3(*this),
			[](PointSimpleListSearcher3 *o) { delete o; }
	);
}
