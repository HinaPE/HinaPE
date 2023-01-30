#ifndef HINAPE_BBOX_H
#define HINAPE_BBOX_H

#include "vector.h"

#include <limits>

namespace Hina::Base
{
template<typename T>
class BoundingBox3
{
public:
	auto width() const -> T;
	auto height() const -> T;
	auto depth() const -> T;
	auto length(size_t axis) -> T;
	auto overlaps(const BoundingBox3 &other) const -> bool;
	auto corner(size_t idx) const -> Vector3<T>;
	auto center() const -> Vector3<T>;

public:
	constexpr BoundingBox3();
	constexpr BoundingBox3(const Vector3<T> &point1, const Vector3<T> &point2);
	constexpr BoundingBox3(const BoundingBox3 &bbox) = default;
	constexpr BoundingBox3(BoundingBox3 &&bbox) noexcept = default;
	auto operator=(const BoundingBox3 &bbox) -> BoundingBox3 & = default;
	auto operator=(BoundingBox3 &&bbox) noexcept -> BoundingBox3 & = default;

public:
	Vector3<T> _lower_corner, _upper_corner;
};
template<typename T>
auto BoundingBox3<T>::center() const -> Vector3<T> { return (_lower_corner + _upper_corner) / static_cast<T>(2); }
template<typename T>
auto BoundingBox3<T>::corner(size_t idx) const -> Vector3<T>
{
	static const T h = static_cast<T>(1) / 2;
	static const std::array<Vector3<T>, 8> offset = {
			Vector3 < T > (-h, -h, -h),
			Vector3 < T > (+h, -h, -h),
			Vector3 < T > (-h, +h, -h),
			Vector3 < T > (+h, +h, -h),
			Vector3 < T > (-h, -h, +h),
			Vector3 < T > (+h, -h, +h),
			Vector3 < T > (-h, +h, +h),
			Vector3 < T > (+h, +h, +h)
	};

	return Vector3 < T > (width(), height(), depth()) * offset[idx] + center();
}

template<typename T>
constexpr BoundingBox3<T>::BoundingBox3() : _lower_corner(0), _upper_corner(0) {}
template<typename T>
constexpr BoundingBox3<T>::BoundingBox3(const Vector3<T> &point1, const Vector3<T> &point2)
{
	_lower_corner.x() = std::min(point1.x(), point2.x());
	_lower_corner.y() = std::min(point1.y(), point2.y());
	_lower_corner.z() = std::min(point1.z(), point2.z());
	_upper_corner.x() = std::max(point1.x(), point2.x());
	_upper_corner.y() = std::max(point1.y(), point2.y());
	_upper_corner.z() = std::max(point1.z(), point2.z());
}

template<typename T>
auto BoundingBox3<T>::width() const -> T { return _upper_corner.x() - _lower_corner.x(); }
template<typename T>
auto BoundingBox3<T>::height() const -> T { return _upper_corner.y() - _lower_corner.y(); }
template<typename T>
auto BoundingBox3<T>::depth() const -> T { return _upper_corner.z() - _lower_corner.z(); }
template<typename T>
auto BoundingBox3<T>::length(size_t axis) -> T
{
	assert(axis < 3);
	return _upper_corner[axis] - _lower_corner[axis];
}
template<typename T>
auto BoundingBox3<T>::overlaps(const BoundingBox3 &other) const -> bool
{
	if (_upper_corner.x() < other._lower_corner.x() || _lower_corner.x() > other._upper_corner.x())
		return false;

	if (_upper_corner.y() < other._lower_corner.y() || _lower_corner.y() > other._upper_corner.y())
		return false;

	if (_upper_corner.z() < other._lower_corner.z() || _lower_corner.z() > other._upper_corner.z())
		return false;

	return true;
}
}

#endif //HINAPE_BBOX_H
