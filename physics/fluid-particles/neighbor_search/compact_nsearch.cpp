#include "compact_nsearch.h"

void HinaPE::CompactNSearch3::for_each_nearby_point(const mVector3 &origin, real radius, const HinaPE::PointNeighborSearch3::ForEachNearbyPointFunc &callback)
{
	std::vector<std::vector<unsigned int>> neighbors;
	_searcher->set_radius(radius);
	_searcher->find_neighbors(&origin[0], neighbors);
	for (unsigned int i: neighbors[0])
		callback(i, _points[i]);
}

auto HinaPE::CompactNSearch3::has_nearby_point(const mVector3 &origin, real radius) const -> bool
{
	// TODO: to optimise this
	std::vector<std::vector<unsigned int>> neighbors;
	_searcher->find_neighbors(&origin[0], neighbors);
	return !neighbors[0].empty();
}

void HinaPE::CompactNSearch3::build(const std::vector<mVector3> &points)
{
	_points.resize(points.size());
	std::copy(points.begin(), points.end(), _points.begin());

	_searcher = std::make_shared<::CompactNSearch::NeighborhoodSearch>(_points.size(), true);
	auto id = _searcher->add_point_set(_points.front().data(), _points.size(), true, true, true, this);
	_searcher->find_neighbors();
	_searcher->update_point_sets();

	_searcher->point_set(id).neighbor_list();
}
