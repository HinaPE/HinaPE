#ifndef HINAPE_VECTOR_H
#define HINAPE_VECTOR_H

#ifdef HINAPE_EIGEN
#include "Eigen/Eigen"
#endif
#include <type_traits>

namespace Hina::Base
{
template<typename T>
class Vector3 final
{
public:
	auto length() -> T;
	void normalize();
	auto x() -> T&;
	auto y() -> T&;
	auto z() -> T&;
	auto x() const -> const T&;
	auto y() const -> const T&;
	auto z() const -> const T&;
	auto at(size_t i) -> T&;
	auto at(size_t i) const -> const T &;
    auto dot(const Vector3 &v) const -> T;
    auto cross(const Vector3 &v) const -> Vector3;

public:
	template<typename U>
	Vector3(const std::initializer_list<U> &lst);
	constexpr Vector3();
	constexpr explicit Vector3(T s);
	constexpr Vector3(T x_, T y_, T z_);
	constexpr Vector3(const Vector3 &v);
	constexpr Vector3(Vector3 &&v) noexcept;

public:
    template<typename U>
    auto operator=(const std::initializer_list<U> &lst) -> Vector3 &;
	auto operator=(const Vector3 &v_) -> Vector3 &;
	auto operator[](size_t i) -> T &;
	auto operator[](size_t i) const -> const T &;
    auto operator+=(T v_) -> Vector3 &;
    auto operator+=(const Vector3 &v_) -> Vector3 &;
    auto operator-=(T v_) -> Vector3 &;
    auto operator-=(const Vector3 &v_) -> Vector3 &;
    auto operator*=(T v_) -> Vector3 &;
    auto operator*=(const Vector3 &v_) -> Vector3 &;
    auto operator/=(T v_) -> Vector3 &;
    auto operator/=(const Vector3 &v_) -> Vector3 &;
    auto operator==(const Vector3 &v_) const -> bool;
    auto operator!=(const Vector3 &v_) const -> bool;

private:
#ifdef HINAPE_EIGEN
	Eigen::Matrix<T, 3, 1> v;
	constexpr explicit Vector3(Eigen::Matrix<T, 3, 1> v_);
#else
	T x, y, z;
#endif
};

#ifdef HINAPE_EIGEN
#include "base/impl/vector_impl_eigen.h"
#else
#endif
}

#endif //HINAPE_VECTOR_H
