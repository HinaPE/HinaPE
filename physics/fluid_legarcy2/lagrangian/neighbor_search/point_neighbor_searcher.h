#ifndef HINAPE_POINT_NEIGHBOR_SEARCHER_H
#define HINAPE_POINT_NEIGHBOR_SEARCHER_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "common.h"

namespace HinaPE
{
class PointNeighborSearcher3
{
public:
	using ForEachNearbyPointFunc = std::function<void(size_t, const mVector3 &)>;

	virtual void for_each_nearby_point(const mVector3 &origin, real radius, const ForEachNearbyPointFunc &callback) = 0;
	virtual auto has_nearby_point(const mVector3 &origin, real radius) const -> bool = 0;
	virtual void build(const std::vector<mVector3> &points) = 0;
};
using PointNeighborSearcher3Ptr = std::shared_ptr<PointNeighborSearcher3>;
} //namespace HinaPE

#endif //HINAPE_POINT_NEIGHBOR_SEARCHER_H
