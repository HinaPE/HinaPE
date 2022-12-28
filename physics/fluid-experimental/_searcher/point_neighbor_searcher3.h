#ifndef HINAPE_POINT_NEIGHBOR_SEARCHER3_H
#define HINAPE_POINT_NEIGHBOR_SEARCHER3_H

#include "math_api.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace HinaPE
{
class PointNeighborSearcher3
{
	virtual auto type_name() const -> std::string = 0;
	virtual void build(const ConstArrayAccessor1<mVector3> &points) = 0;
	virtual void for_each_nearby_point(const mVector3 &origin, real radius, const std::function<void(size_t, const mVector3 &)> &callback) const = 0;
	virtual auto has_nearby_point(const mVector3 &origin, real radius) const -> bool = 0;
	virtual auto clone() const -> std::shared_ptr<PointNeighborSearcher3> = 0;
};
using PointNeighborSearcher3Ptr = std::shared_ptr<PointNeighborSearcher3>;
}

#endif //HINAPE_POINT_NEIGHBOR_SEARCHER3_H
