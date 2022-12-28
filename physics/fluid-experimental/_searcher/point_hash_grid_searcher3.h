#ifndef HINAPE_POINT_HASH_GRID_SEARCHER3_H
#define HINAPE_POINT_HASH_GRID_SEARCHER3_H

#include "point_neighbor_searcher3.h"
namespace HinaPE
{
class PointHashGridSearcher3 : public PointNeighborSearcher3
{
	auto type_name() const -> std::string override;
	void build(const ConstArrayAccessor1<mVector3> &points) override;
	void for_each_nearby_point(const mVector3 &origin, real radius, const std::function<void(size_t, const mVector3 &)> &callback) const override;
	auto has_nearby_point(const mVector3 &origin, real radius) const -> bool override;
	auto clone() const -> std::shared_ptr<PointNeighborSearcher3> override;
};
using PointHashGridSearcher3Ptr = std::shared_ptr<PointHashGridSearcher3>;
}

#endif //HINAPE_POINT_HASH_GRID_SEARCHER3_H
