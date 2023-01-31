#ifndef HINAPE_ARRAY_H
#define HINAPE_ARRAY_H

#include "size.h"
#include "vector.h"
#include "math_ext.h"
#include <vector>

namespace Hina::Base
{
template<typename T>
class Array1
{
private:
	std::vector<T> _data;
};

template<typename T>
class Array2
{
private:
	std::vector<T> _data;
};

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
	return nullptr;
}
}
#endif //HINAPE_ARRAY_H
