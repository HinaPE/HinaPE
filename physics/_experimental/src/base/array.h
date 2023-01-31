#ifndef HINAPE_ARRAY_H
#define HINAPE_ARRAY_H

#include "size.h"
#include "vector.h"
#include "math_ext.h"
#include "parallel.h"
#include <vector>

namespace Hina::Base
{
template<typename T>
class Array3
{
public:
	inline void resize(const Size3 &size, const T &initial_value = T())
	{
		Array3<T> grid;
		grid._data.resize(size.x * size.y * size.z, initial_value);
		grid._size = size;
		size_t i_min = std::min(size.x, _size.x);
		size_t j_min = std::min(size.y, _size.y);
		size_t k_min = std::min(size.z, _size.z);
		for (size_t k = 0; k < k_min; ++k)
			for (size_t j = 0; j < j_min; ++j)
				for (size_t i = 0; i < i_min; ++i)
					grid(i, j, k) = at(i, j, k);
		swap(grid);
	}
	inline auto swap(Array3<T> &other) -> void
	{
		std::swap(_data, other._data);
		std::swap(_size, other._size);
	}
	inline auto size() const -> const Size3 & { return _size; }

public:
	inline auto at(size_t i, size_t j, size_t k) -> T & { return _data[i + j * _size.x + k * _size.x * _size.y]; }
	inline auto at(size_t i, size_t j, size_t k) const -> const T & { return _data[i + j * _size.x + k * _size.x * _size.y]; }
	inline auto operator()(size_t i, size_t j, size_t k) -> T & { return _data[i + j * _size.x + k * _size.x * _size.y]; }
	inline auto operator()(size_t i, size_t j, size_t k) const -> const T & { return _data[i + j * _size.x + k * _size.x * _size.y]; }

private:
	std::vector<T> _data;
	Size3 _size;
};

template<typename T, typename R>
class LinearArray3Sampler final
{
public:
	constexpr explicit LinearArray3Sampler(const Array3<T> &accessor) : _accessor(accessor) {}
	auto operator()(const Vector3<R> &pt) const -> T;

private:
	Vector3<R> _grid_spacing;
	Vector3<R> _inv_grid_spacing;
	Vector3<R> _origin;
	const Array3<T> &_accessor;
};

template<typename T, typename R>
auto LinearArray3Sampler<T, R>::operator()(const Vector3<R> &pt) const -> T
{
	long i, j, k;
	R fx, fy, fz;

	Vector3<R> normalized_pt = (pt - _origin) * _inv_grid_spacing;

	long i_size = static_cast<long>(_accessor.size().x);
	long j_size = static_cast<long>(_accessor.size().y);
	long k_size = static_cast<long>(_accessor.size().z);

	get_barycentric(normalized_pt.x, 0, i_size - 1, &i, &fx);
	get_barycentric(normalized_pt.y, 0, j_size - 1, &j, &fy);
	get_barycentric(normalized_pt.z, 0, k_size - 1, &k, &fz);

	long ip1 = std::min(i + 1, i_size - 1);
	long jp1 = std::min(j + 1, j_size - 1);
	long kp1 = std::min(k + 1, k_size - 1);

	return trilerp(
			_accessor(i, j, k),
			_accessor(ip1, j, k),
			_accessor(i, jp1, k),
			_accessor(ip1, jp1, k),
			_accessor(i, j, kp1),
			_accessor(ip1, j, kp1),
			_accessor(i, jp1, kp1),
			_accessor(ip1, jp1, kp1),
			fx,
			fy,
			fz);
}
}
#endif //HINAPE_ARRAY_H
