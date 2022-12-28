#ifndef HINAPE_POINT_SIMPLE_LIST_SEARCHER3_H
#define HINAPE_POINT_SIMPLE_LIST_SEARCHER3_H

#include "point_neighbor_searcher3.h"
namespace HinaPE
{
class PointSimpleListSearcher3 : public PointNeighborSearcher3
{
public:
	auto type_name() const -> std::string override;
	void build(const ConstArrayAccessor1<mVector3> &points) override;
	void for_each_nearby_point(const mVector3 &origin, real radius, const std::function<void(size_t, const mVector3 &)> &callback) const override;
	auto has_nearby_point(const mVector3 &origin, real radius) const -> bool override;
	auto clone() const -> std::shared_ptr<PointNeighborSearcher3> override;

private:
	std::vector<mVector3> _points;
};
using PointSimpleListSearcher3Ptr = std::shared_ptr<PointSimpleListSearcher3>;
}

#endif //HINAPE_POINT_SIMPLE_LIST_SEARCHER3_H
