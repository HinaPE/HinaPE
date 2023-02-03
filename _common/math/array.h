#ifndef HINAPE_ARRAY_H
#define HINAPE_ARRAY_H

#include "math/size.h"
#include "math/vector.h"
#include "math/math_ext.h"
#include "util/parallel.h"

#include <vector>
#include <array>

namespace HinaPE::Math
{
// ============================== Array3 ==============================
template<typename T>
class Array3
{
public:
	void resize(const Size3 &size, const T &initial_value = T());
	auto swap(Array3<T> &other) -> void;

public:
	inline auto at(size_t i, size_t j, size_t k) -> T & { return _data[i + j * _size.x + k * _size.x * _size.y]; }
	inline auto at(size_t i, size_t j, size_t k) const -> const T & { return _data[i + j * _size.x + k * _size.x * _size.y]; }
	inline auto operator()(size_t i, size_t j, size_t k) -> T & { return _data[i + j * _size.x + k * _size.x * _size.y]; }
	inline auto operator()(size_t i, size_t j, size_t k) const -> const T & { return _data[i + j * _size.x + k * _size.x * _size.y]; }
	inline auto size() const -> const Size3 & { return _size; }

public:
	template<typename Callback>
	void for_each(const Callback &callback) const { for (size_t k = 0; k < _size.z; ++k) for (size_t j = 0; j < _size.y; ++j) for (size_t i = 0; i < _size.x; ++i) callback(at(i, j, k)); }
	template<typename Callback>
	void parallel_for_each(const Callback &callback) const { Util::parallelFor((size_t) 0, _size.x, (size_t) 0, _size.y, (size_t) 0, _size.z, [&](size_t i, size_t j, size_t k) { callback(at(i, j, k)); }); }
	template<typename Callback>
	void for_each_index(const Callback &callback) const { for (size_t k = 0; k < _size.z; ++k) for (size_t j = 0; j < _size.y; ++j) for (size_t i = 0; i < _size.x; ++i) callback(i, j, k); }
	template<typename Callback>
	void parallel_for_each_index(const Callback &callback) const { Util::parallelFor((size_t) 0, _size.x, (size_t) 0, _size.y, (size_t) 0, _size.z, callback); }

private:
	std::vector<T> _data;
	Size3 _size;
};

template<typename T>
void Array3<T>::resize(const Size3 &size, const T &initial_value)
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

template<typename T>
auto Array3<T>::swap(Array3<T> &other) -> void
{
	std::swap(_data, other._data);
	std::swap(_size, other._size);
}


// ============================== LinearArray3Sampler ==============================
template<typename T, typename R>
class LinearArray3Sampler final
{
public:
	constexpr explicit LinearArray3Sampler(const Array3<T> &accessor) : _accessor(accessor) {}
	auto operator()(const Vector3<R> &pt) const -> T;
	auto functor() -> std::function<T(const Vector3<R> &)>;
	void get_coordinate_and_weights(const Vector3<R> &pt, std::array<Vector3<size_t>, 8> *indices, std::array<R, 8> *weights) const;
	void get_coordinate_and_gradient_weights(const Vector3<R> &pt, std::array<Vector3<size_t>, 8> *indices, std::array<Vector3<R>, 8> *weights) const;

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

	get_barycentric(normalized_pt.x(), 0, i_size - 1, &i, &fx);
	get_barycentric(normalized_pt.y(), 0, j_size - 1, &j, &fy);
	get_barycentric(normalized_pt.z(), 0, k_size - 1, &k, &fz);

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

template<typename T, typename R>
auto LinearArray3Sampler<T, R>::functor() -> std::function<T(const Vector3<R> &)> { return std::bind(&LinearArray3Sampler<T, R>::operator(), this, std::placeholders::_1); }

template<typename T, typename R>
void LinearArray3Sampler<T, R>::get_coordinate_and_weights(const Vector3<R> &pt, std::array<Vector3<size_t>, 8> *indices, std::array<R, 8> *weights) const
{
	long i, j, k;
	R fx, fy, fz;

	const Vector3<R> normalized_pt = (pt - _origin) * _inv_grid_spacing;

	const long i_size = static_cast<long>(_accessor.size().x);
	const long j_size = static_cast<long>(_accessor.size().y);
	const long k_size = static_cast<long>(_accessor.size().z);

	get_barycentric(normalized_pt.x(), 0, i_size - 1, &i, &fx);
	get_barycentric(normalized_pt.y(), 0, j_size - 1, &j, &fy);
	get_barycentric(normalized_pt.z(), 0, k_size - 1, &k, &fz);

	const long ip1 = std::min(i + 1, i_size - 1);
	const long jp1 = std::min(j + 1, j_size - 1);
	const long kp1 = std::min(k + 1, k_size - 1);

	(*indices)[0] = Vector3<size_t>(i, j, k);
	(*indices)[1] = Vector3<size_t>(ip1, j, k);
	(*indices)[2] = Vector3<size_t>(i, jp1, k);
	(*indices)[3] = Vector3<size_t>(ip1, jp1, k);
	(*indices)[4] = Vector3<size_t>(i, j, kp1);
	(*indices)[5] = Vector3<size_t>(ip1, j, kp1);
	(*indices)[6] = Vector3<size_t>(i, jp1, kp1);
	(*indices)[7] = Vector3<size_t>(ip1, jp1, kp1);

	(*weights)[0] = (1 - fx) * (1 - fy) * (1 - fz);
	(*weights)[1] = fx * (1 - fy) * (1 - fz);
	(*weights)[2] = (1 - fx) * fy * (1 - fz);
	(*weights)[3] = fx * fy * (1 - fz);
	(*weights)[4] = (1 - fx) * (1 - fy) * fz;
	(*weights)[5] = fx * (1 - fy) * fz;
	(*weights)[6] = (1 - fx) * fy * fz;
	(*weights)[7] = fx * fy * fz;
}

template<typename T, typename R>
void LinearArray3Sampler<T, R>::get_coordinate_and_gradient_weights(const Vector3<R> &pt, std::array<Vector3<size_t>, 8> *indices, std::array<Vector3<R>, 8> *weights) const
{
	long i, j, k;
	R fx, fy, fz;

	const Vector3<R> normalized_pt = (pt - _origin) * _inv_grid_spacing;

	const long i_size = static_cast<long>(_accessor.size().x);
	const long j_size = static_cast<long>(_accessor.size().y);
	const long k_size = static_cast<long>(_accessor.size().z);

	get_barycentric(normalized_pt.x(), 0, i_size - 1, &i, &fx);
	get_barycentric(normalized_pt.y(), 0, j_size - 1, &j, &fy);
	get_barycentric(normalized_pt.z(), 0, k_size - 1, &k, &fz);

	const long ip1 = std::min(i + 1, i_size - 1);
	const long jp1 = std::min(j + 1, j_size - 1);
	const long kp1 = std::min(k + 1, k_size - 1);

	(*indices)[0] = Vector3<size_t>(i, j, k);
	(*indices)[1] = Vector3<size_t>(ip1, j, k);
	(*indices)[2] = Vector3<size_t>(i, jp1, k);
	(*indices)[3] = Vector3<size_t>(ip1, jp1, k);
	(*indices)[4] = Vector3<size_t>(i, j, kp1);
	(*indices)[5] = Vector3<size_t>(ip1, j, kp1);
	(*indices)[6] = Vector3<size_t>(i, jp1, kp1);
	(*indices)[7] = Vector3<size_t>(ip1, jp1, kp1);

	(*weights)[0] = Vector3<R>(-_inv_grid_spacing.x * (1 - fy) * (1 - fz), -_inv_grid_spacing.y * (1 - fx) * (1 - fz), -_inv_grid_spacing.z * (1 - fx) * (1 - fy));
	(*weights)[1] = Vector3<R>(_inv_grid_spacing.x * (1 - fy) * (1 - fz), fx * (-_inv_grid_spacing.y) * (1 - fz), fx * (1 - fy) * (-_inv_grid_spacing.z));
	(*weights)[2] = Vector3<R>((-_inv_grid_spacing.x) * fy * (1 - fz), (1 - fx) * _inv_grid_spacing.y * (1 - fz), (1 - fx) * fy * (-_inv_grid_spacing.z));
	(*weights)[3] = Vector3<R>(_inv_grid_spacing.x * fy * (1 - fz), fx * _inv_grid_spacing.y * (1 - fz), fx * fy * (-_inv_grid_spacing.z));
	(*weights)[4] = Vector3<R>((-_inv_grid_spacing.x) * (1 - fy) * fz, (1 - fx) * (-_inv_grid_spacing.y) * fz, (1 - fx) * (1 - fy) * _inv_grid_spacing.z);
	(*weights)[5] = Vector3<R>(_inv_grid_spacing.x * (1 - fy) * fz, fx * (-_inv_grid_spacing.y) * fz, fx * (1 - fy) * _inv_grid_spacing.z);
	(*weights)[6] = Vector3<R>((-_inv_grid_spacing.x) * fy * fz, (1 - fx) * _inv_grid_spacing.y * fz, (1 - fx) * fy * _inv_grid_spacing.z);
	(*weights)[7] = Vector3<R>(_inv_grid_spacing.x * fy * fz, fx * _inv_grid_spacing.y * fz, fx * fy * _inv_grid_spacing.z);
}

// FDM
template<typename T>
auto gradient3(const Array3<T> &data, const Vector3<T> &grid_spacing, size_t i, size_t j, size_t k) -> Vector3<T>
{
	const Size3 ds = data.size();
	assert(i < ds.x && j < ds.y && k < ds.z);

	T left = data((i > 0) ? i - 1 : i, j, k);
	T right = data((i + 1 < ds.x) ? i + 1 : i, j, k);
	T down = data(i, (j > 0) ? j - 1 : j, k);
	T up = data(i, (j + 1 < ds.y) ? j + 1 : j, k);
	T back = data(i, j, (k > 0) ? k - 1 : k);
	T front = data(i, j, (k + 1 < ds.z) ? k + 1 : k);

	return static_cast<T>(0.5) * Vector3<T>(right - left, up - down, front - back) / grid_spacing;
}
template<typename T>
auto gradient3(const Array3<Vector3<T>> &data, const Vector3<T> &grid_spacing, size_t i, size_t j, size_t k) -> std::array<Vector3<T>, 3>
{
	const Size3 ds = data.size();
	assert(i < ds.x && j < ds.y && k < ds.z);

	Vector3<T> left = data((i > 0) ? i - 1 : i, j, k);
	Vector3<T> right = data((i + 1 < ds.x) ? i + 1 : i, j, k);
	Vector3<T> down = data(i, (j > 0) ? j - 1 : j, k);
	Vector3<T> up = data(i, (j + 1 < ds.y) ? j + 1 : j, k);
	Vector3<T> back = data(i, j, (k > 0) ? k - 1 : k);
	Vector3<T> front = data(i, j, (k + 1 < ds.z) ? k + 1 : k);

	std::array<Vector3<T>, 3> result;
	result[0] = 0.5 * Vector3<T>(right.x() - left.x(), up.x() - down.x(), front.x() - back.x()) / grid_spacing;
	result[1] = 0.5 * Vector3<T>(right.y() - left.y(), up.y() - down.y(), front.y() - back.y()) / grid_spacing;
	result[2] = 0.5 * Vector3<T>(right.z() - left.z(), up.z() - down.z(), front.z() - back.z()) / grid_spacing;
	return result;
}
template<typename T>
auto laplacian3(const Array3<T> &data, const Vector3<T> &grid_spacing, size_t i, size_t j, size_t k) -> T
{
	const T center = data(i, j, k);
	const Size3 ds = data.size();
	assert(i < ds.x && j < ds.y && k < ds.z);

	T d_left = static_cast<T>(0.0);
	T d_right = static_cast<T>(0.0);
	T d_down = static_cast<T>(0.0);
	T d_up = static_cast<T>(0.0);
	T d_back = static_cast<T>(0.0);
	T d_front = static_cast<T>(0.0);

	if (i > 0) d_left = center - data(i - 1, j, k);
	if (i + 1 < ds.x) d_right = data(i + 1, j, k) - center;
	if (j > 0) d_down = center - data(i, j - 1, k);
	if (j + 1 < ds.y) d_up = data(i, j + 1, k) - center;
	if (k > 0) d_back = center - data(i, j, k - 1);
	if (k + 1 < ds.z) d_front = data(i, j, k + 1) - center;

	return (d_right - d_left) / square(grid_spacing.x()) + (d_up - d_down) / square(grid_spacing.y()) + (d_front - d_back) / square(grid_spacing.z());
}
template<typename T>
auto laplacian3(const Array3<Vector3<T>> &data, const Vector3<T> &grid_spacing, size_t i, size_t j, size_t k) -> Vector3<T>
{
	const Vector3<T> center = data(i, j, k);
	const Size3 ds = data.size();

	Vector3<T> d_left = Vector3<T>::Zero();
	Vector3<T> d_right = Vector3<T>::Zero();
	Vector3<T> d_down = Vector3<T>::Zero();
	Vector3<T> d_up = Vector3<T>::Zero();
	Vector3<T> d_back = Vector3<T>::Zero();
	Vector3<T> d_front = Vector3<T>::Zero();

	if (i > 0) d_left = center - data(i - 1, j, k);
	if (i + 1 < ds.x) d_right = data(i + 1, j, k) - center;
	if (j > 0) d_down = center - data(i, j - 1, k);
	if (j + 1 < ds.y) d_up = data(i, j + 1, k) - center;
	if (k > 0) d_back = center - data(i, j, k - 1);
	if (k + 1 < ds.z) d_front = data(i, j, k + 1) - center;

	return (d_right - d_left) / square(grid_spacing.x()) + (d_up - d_down) / square(grid_spacing.y()) + (d_front - d_back) / square(grid_spacing.z());
}
}
#endif //HINAPE_ARRAY_H
