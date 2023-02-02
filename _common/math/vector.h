#ifndef HINAPE_VECTOR_H
#define HINAPE_VECTOR_H

#ifdef HINA_EIGEN
#include "Eigen/Eigen"
#endif
#include <type_traits>

#include "math_ext.h"

namespace HinaPE::Math
{
template<typename T>
class Vector3 final
{
public:
	auto x() -> T &;
	auto y() -> T &;
	auto z() -> T &;
	auto x() const -> const T &;
	auto y() const -> const T &;
	auto z() const -> const T &;
	auto at(size_t i) -> T &;
	auto at(size_t i) const -> const T &;
	auto sum() const -> T;
	auto avg() const -> T;
	auto min() const -> T;
	auto max() const -> T;
	auto dot(const Vector3 &v) const -> T;
	auto cross(const Vector3 &v) const -> Vector3;
	auto length() -> T;
	auto length_squared() -> T;
	void normalize();

public:
	static inline constexpr auto Zero() -> Vector3 { return Vector3(0, 0, 0); }

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

public:
#ifdef HINA_EIGEN
	constexpr explicit Vector3(Eigen::Matrix<T, 3, 1, Eigen::DontAlign> v_);
	template<typename U>
	Vector3(const std::initializer_list<U> &lst);
	constexpr Vector3();
	constexpr explicit Vector3(T s);
	constexpr Vector3(T x_, T y_, T z_);
	constexpr Vector3(const Vector3 &v);
	constexpr Vector3(Vector3 &&v) noexcept;
	~Vector3() = default;
	Eigen::Matrix<T, 3, 1, Eigen::DontAlign> _v;
#else
	T x, y, z;
#endif
};

//@formatter:off
template<typename T> auto operator+(const Vector3<T>& a) -> Vector3<T> { return a;}
template<typename T> auto operator-(const Vector3<T>& a) -> Vector3<T> { return {-a.x(), -a.y(), -a.z()}; }
template<typename T> auto operator+(const Vector3<T>& a, T b) -> Vector3<T> { return { a.x() + b, a.y() + b, a.z() + b}; }
template<typename T> auto operator+(T a, const Vector3<T>& b) -> Vector3<T> { return { a + b.x(), a + b.y(), a + b.z()}; }
template<typename T> auto operator+(const Vector3<T>& a, const Vector3<T>& b) -> Vector3<T> { return { a.x() + b.x(), a.y() + b.y(), a.z() + b.z()}; }
template<typename T> auto operator-(const Vector3<T>& a, T b) -> Vector3<T> { return { a.x() - b, a.y() - b, a.z() - b}; }
template<typename T> auto operator-(T a, const Vector3<T>& b) -> Vector3<T> { return { a - b.x(), a - b.y(), a - b.z()}; }
template<typename T> auto operator-(const Vector3<T>& a, const Vector3<T>& b) -> Vector3<T> { return { a.x() - b.x(), a.y() - b.y(), a.z() - b.z()}; }
template<typename T> auto operator*(const Vector3<T>& a, T b) -> Vector3<T> { return { a.x() * b, a.y() * b, a.z() * b}; }
template<typename T> auto operator*(T a, const Vector3<T>& b) -> Vector3<T> { return { a * b.x(), a * b.y(), a * b.z()}; }
template<typename T> auto operator*(const Vector3<T>& a, const Vector3<T>& b) -> Vector3<T> { return { a.x() * b.x(), a.y() * b.y(), a.z() * b.z()}; }
template<typename T> auto operator/(const Vector3<T>& a, T b) -> Vector3<T> { return { a.x() / b, a.y() / b, a.z() / b}; }
template<typename T> auto operator/(T a, const Vector3<T>& b) -> Vector3<T> { return { a / b.x(), a / b.y(), a / b.z()}; }
template<typename T> auto operator/(const Vector3<T>& a, const Vector3<T>& b) -> Vector3<T> { return { a.x() / b.x(), a.y() / b.y(), a.z() / b.z()}; }
template<typename T> auto max(const Vector3<T> &a, const Vector3<T> &b) -> Vector3<T> { return { std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()) };}
template<typename T> auto min(const Vector3<T> &a, const Vector3<T> &b) -> Vector3<T>{ return { std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()) };}
template<typename T> auto clamp(const Vector3<T> &v, const Vector3<T> &low, const Vector3<T> &high) -> Vector3<T> { return {clamp(v.x(), low.x(), high.x()), clamp(v.y(), low.y(), high.y()), clamp(v.z(), low.z(), high.z())}; }
template<typename T> auto ceil(const Vector3<T> &a) -> Vector3<T> { return {std::ceil(a.x()), std::ceil(a.y()), std::ceil(a.z())}; }
template<typename T> auto floor(const Vector3<T> &a) -> Vector3<T> { return {std::floor(a.x()), std::floor(a.y()), std::floor(a.z())}; }
//@formatter:on
}
#ifdef HINA_EIGEN
#include "impl/vector_impl_eigen.h"
#else
#endif

#ifdef HINAPE_DOUBLE
using mVector3 = HinaPE::Math::Vector3<double>;
#else
using Vector3 = Math::Vector3<float>;
#endif
#endif //HINAPE_VECTOR_H
