#include "point_hash_grid_searcher3.h"
std::string HinaPE::PointHashGridSearcher3::type_name() const
{
	return "PointHashGridSearcher3";
}
void HinaPE::PointHashGridSearcher3::build(const HinaPE::ConstArrayAccessor1<mVector3> &points)
{
}
void HinaPE::PointHashGridSearcher3::for_each_nearby_point(const mVector3 &origin, real radius, const std::function<void(size_t, const mVector3 &)> &callback) const
{
}
auto HinaPE::PointHashGridSearcher3::has_nearby_point(const mVector3 &origin, real radius) const -> bool
{
	return false;
}
auto HinaPE::PointHashGridSearcher3::clone() const -> std::shared_ptr<PointNeighborSearcher3>
{
	return std::shared_ptr<PointHashGridSearcher3>(
			new PointHashGridSearcher3(*this),
			[](PointHashGridSearcher3 *p) { delete p; }
	);
}
