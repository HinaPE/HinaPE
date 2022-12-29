#ifndef HINAPE_POINT_HASH_GRID_SEARCHER3_H
#define HINAPE_POINT_HASH_GRID_SEARCHER3_H

#include "point_neighbor_searcher3.h"

#include "array/array1.h"
namespace HinaPE
{
class PointHashGridSearcher3 : public PointNeighborSearcher3
{
public:
	auto type_name() const -> std::string final;
	void build(const ConstArrayAccessor1<mVector3> &points) final;
	void for_each_nearby_point(const mVector3 &origin, real radius, const std::function<void(size_t, const mVector3 &)> &callback) const final;
	auto has_nearby_point(const mVector3 &origin, real radius) const -> bool final;
	auto clone() const -> std::shared_ptr<PointNeighborSearcher3> final;

public:
	void add(const mVector3 &point);

protected:
	auto get_hash_key_from_position(const mVector3 &position) -> size_t;
	auto get_hash_key_from_bucket_index(const mPoint3I &bucket_index) const -> size_t;
	auto get_bucket_index(const mVector3 &position) const -> mPoint3I;
	auto get_nearby_keys(const mVector3 &position) const -> std::array<size_t, 8>;

private:
	real _grid_spacing = 1.0;
	mPoint3I _resolution;
	std::vector<mVector3> _points;
	std::vector<std::vector<size_t>> _buckets;
};
using PointHashGridSearcher3Ptr = std::shared_ptr<PointHashGridSearcher3>;
}

#endif //HINAPE_POINT_HASH_GRID_SEARCHER3_H
