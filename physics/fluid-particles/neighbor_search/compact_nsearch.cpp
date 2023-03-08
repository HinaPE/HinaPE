#include "compact_nsearch.h"
void HinaPE::CompactNSearch3::for_each_nearby_point(const mVector3 &origin, real radius, const HinaPE::PointNeighborSearch3::ForEachNearbyPointFunc &callback)
{
	std::vector<std::vector<unsigned int>> neighbors;
	_searcher->find_neighbors(&origin[0], neighbors);
}
auto HinaPE::CompactNSearch3::has_nearby_point(const mVector3 &origin, real radius) const -> bool
{
	// TODO: to optimise this
	std::vector<std::vector<unsigned int>> neighbors;
	_searcher->find_neighbors(&origin[0], neighbors);
	return !neighbors.empty();
}
void HinaPE::CompactNSearch3::build(const std::vector<mVector3> &points)
{
	_searcher = std::make_shared<::CompactNSearch::NeighborhoodSearch>(points.size());
	_searcher->add_point_set(&points[0][0], points.size(), true, true, true, this);
	_searcher->update_point_sets();
}
