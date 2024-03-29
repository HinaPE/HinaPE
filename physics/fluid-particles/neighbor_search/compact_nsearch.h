#ifndef HINAPE_COMPACT_NSEARCH_H
#define HINAPE_COMPACT_NSEARCH_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "point_neighbor_search.h"
#include "nsearch/CompactNSearch.h"

namespace HinaPE
{
class CompactNSearch3 final : public PointNeighborSearch3
{
public:
	void for_each_nearby_point(const mVector3 &origin, const ForEachNearbyPointFunc &callback) final;
	auto has_nearby_point(const mVector3 &origin) const -> bool final;
	void build(const std::vector<mVector3> &points) final;

	explicit CompactNSearch3(real radius);

private:
	std::shared_ptr<::CompactNSearch::NeighborhoodSearch> _searcher;
	std::vector<mVector3> _points;
	bool _inited = false;
	size_t ID;
};
} //namespace HinaPE

#endif //HINAPE_COMPACT_NSEARCH_H
