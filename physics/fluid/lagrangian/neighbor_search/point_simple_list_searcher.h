#ifndef HINAPE_POINT_SIMPLE_LIST_SEARCHER_H
#define HINAPE_POINT_SIMPLE_LIST_SEARCHER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "point_neighbor_searcher.h"

namespace HinaPE
{
class PointSimpleListSearcher3 : public PointNeighborSearcher3
{
public:
	void for_each_nearby_point(const mVector3 &origin, real radius, const ForEachNearbyPointFunc &callback) final;
	auto has_nearby_point(const mVector3 &origin, real radius) const -> bool final;
	void build(const std::vector<mVector3> &points) final;

private:
	std::vector<mVector3> _points;
};
using PointSimpleListSearcher3Ptr = std::shared_ptr<PointSimpleListSearcher3>;
} //namespace HinaPE

#endif //HINAPE_POINT_SIMPLE_LIST_SEARCHER_H
