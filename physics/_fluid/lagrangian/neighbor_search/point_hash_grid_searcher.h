#ifndef HINAPE_POINT_HASH_GRID_SEARCHER_H
#define HINAPE_POINT_HASH_GRID_SEARCHER_H

#include "point_neighbor_searcher.h"

namespace HinaPE
{
class PointHashGridSearcher3 : public PointNeighborSearcher3
{
public:
	void for_each_nearby_point(const mVector3 &origin, real radius, const ForEachNearbyPointFunc &callback) final;
	auto has_nearby_point(const mVector3 &origin, real radius) const -> bool final;
	void build(const std::vector<mVector3> &points) final;

private:
	real _grid_spacing = 1.0;
	mSize3 _resolution = {1, 1, 1};
	std::vector<mVector3> _points;
	std::vector<std::vector<size_t>> _buckets;
};
} //namespace HinaPE

#endif //HINAPE_POINT_HASH_GRID_SEARCHER_H
