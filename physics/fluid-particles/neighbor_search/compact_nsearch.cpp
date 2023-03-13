#include "compact_nsearch.h"

HinaPE::CompactNSearch3::CompactNSearch3(real radius) : ID(0), PointNeighborSearch3(radius) { _searcher = std::make_shared<::CompactNSearch::NeighborhoodSearch>(radius, true); }

void HinaPE::CompactNSearch3::for_each_nearby_point(const mVector3 &origin, const HinaPE::PointNeighborSearch3::ForEachNearbyPointFunc &callback)
{
	std::vector<std::vector<unsigned int>> neighbors;
	_searcher->set_radius(_radius);
	_searcher->find_neighbors(&origin[0], neighbors);
	for (unsigned int i: neighbors[0])
		callback(i, mVector3());
}

auto HinaPE::CompactNSearch3::has_nearby_point(const mVector3 &origin) const -> bool
{
	// TODO: to optimise this
	std::vector<std::vector<unsigned int>> neighbors;
	_searcher->find_neighbors(&origin[0], neighbors);
	return !neighbors[0].empty();
}

void HinaPE::CompactNSearch3::build(const std::vector<mVector3> &points)
{
	if (!_inited)
	{
		ID = _searcher->add_point_set(points.front().data(), points.size(), true, true, true, this);
		_inited = true;
	}
	_searcher->find_neighbors();
	_searcher->update_point_sets();
}
