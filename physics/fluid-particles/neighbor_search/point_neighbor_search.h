#ifndef HINAPE_POINT_NEIGHBOR_SEARCH_H
#define HINAPE_POINT_NEIGHBOR_SEARCH_H

// Copyright (c) 2023 Xayah Hina
// MPL-2.0 license

#include "common.h"

namespace HinaPE
{
class PointNeighborSearch3
{
public:
	using ForEachNearbyPointFunc = std::function<void(size_t, const mVector3 &)>;

	virtual void for_each_nearby_point(const mVector3 &origin, const ForEachNearbyPointFunc &callback) = 0;
	virtual auto has_nearby_point(const mVector3 &origin) const -> bool = 0;
	virtual void build(const std::vector<mVector3> &points) = 0;

public:
	explicit PointNeighborSearch3(real radius) : _radius(radius) {}

protected:
	real _radius;
};


class PointSimpleListSearch3 final : public PointNeighborSearch3
{
public:
	void for_each_nearby_point(const mVector3 &origin, const ForEachNearbyPointFunc &callback) final;
	auto has_nearby_point(const mVector3 &origin) const -> bool final;
	void build(const std::vector<mVector3> &points) final;
	explicit PointSimpleListSearch3(real radius) : PointNeighborSearch3(radius) {}

private:
	std::vector<mVector3> _points;
};


class PointHashGridSearch3 final : public PointNeighborSearch3
{
public:
	void for_each_nearby_point(const mVector3 &origin, const ForEachNearbyPointFunc &callback) final;
	auto has_nearby_point(const mVector3 &origin) const -> bool final;
	void build(const std::vector<mVector3> &points) final;
	void add_point(const mVector3 &point);
	explicit PointHashGridSearch3(real radius) : PointNeighborSearch3(radius), _grid_spacing(2 * radius) {}

private:
	auto _get_hash_key_from_position(const mVector3 &position) const -> size_t;
	auto _get_nearby_keys(const mVector3 &position) const -> std::array<size_t, 8>;
	auto _get_hash_key_from_bucket_index(const mVector3i &index) const -> size_t;
	auto _get_bucket_index(const mVector3 &position) const -> mVector3i;

private:
	real _grid_spacing = 0.04; // 2 * kernel radius
	mSize3 _resolution = {10, 10, 10};
	std::vector<mVector3> _points;
	std::vector<std::vector<size_t>> _buckets;
};

class PointParallelHashGridSearch3 final : public PointNeighborSearch3
{
public:
	void for_each_nearby_point(const mVector3 &origin, const ForEachNearbyPointFunc &callback) final;
	auto has_nearby_point(const mVector3 &origin) const -> bool final;
	void build(const std::vector<mVector3> &points) final;
	explicit PointParallelHashGridSearch3(real radius) : PointNeighborSearch3(radius), _grid_spacing(2 * radius) {}

private:
	auto _get_hash_key_from_position(const mVector3 &position) const -> size_t;
	auto _get_nearby_keys(const mVector3 &position) const -> std::array<size_t, 8>;
	auto _get_hash_key_from_bucket_index(const mVector3i &index) const -> size_t;
	auto _get_bucket_index(const mVector3 &position) const -> mVector3i;

private:
	real _grid_spacing = 0.04;
	mSize3 _resolution = {10, 10, 10};
	std::vector<mVector3> _points;
	std::vector<size_t> _keys;
	std::vector<size_t> _start_index_table;
	std::vector<size_t> _end_index_table;
	std::vector<size_t> _sorted_indices;
};

using PointNeighborSearch3Ptr = std::shared_ptr<PointNeighborSearch3>;
using PointSimpleListSearch3Ptr = std::shared_ptr<PointSimpleListSearch3>;
using PointHashGridSearch3Ptr = std::shared_ptr<PointHashGridSearch3>;
} //namespace HinaPE

#endif //HINAPE_POINT_NEIGHBOR_SEARCH_H
