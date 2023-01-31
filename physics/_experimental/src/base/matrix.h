#ifndef HINAPE_MATRIX_H
#define HINAPE_MATRIX_H


#ifdef HINAPE_EIGEN
#include "Eigen/Eigen"
#endif
#include <type_traits>

namespace Hina::Base
{
template<typename T>
class Matrix3x3 final
{
public:
	auto inverse() const -> Matrix3x3;

public:
	static inline constexpr auto Zero() -> Matrix3x3 { return Matrix3x3(Eigen::Matrix<T, 3, 3, Eigen::DontAlign>::Zero()); }
	static inline constexpr auto Identity() -> Matrix3x3 { return Matrix3x3(Eigen::Matrix<T, 3, 3, Eigen::DontAlign>::Identity()); }

public:
#ifdef HINAPE_EIGEN
	template<typename U>
	Matrix3x3(const std::initializer_list<U> &lst);
	constexpr explicit Matrix3x3();
	constexpr explicit Matrix3x3(Eigen::Matrix<T, 3, 3, Eigen::DontAlign> m_);
	constexpr Matrix3x3(const Matrix3x3<T> &m_) = default;
	constexpr Matrix3x3(Matrix3x3<T> &&m_) noexcept = default;
	constexpr auto operator=(const Matrix3x3<T> &m_) -> Matrix3x3<T> & = default;
	constexpr auto operator=(Matrix3x3<T> &&m_) noexcept -> Matrix3x3<T> & = default;
	~Matrix3x3() = default;
	Eigen::Matrix<T, 3,3, Eigen::DontAlign> _m;
#endif
};


template<typename T>
auto operator+(const Matrix3x3<T> &a) -> Matrix3x3<T> { return Matrix3x3<T>(a._m); }
template<typename T>
auto operator-(const Matrix3x3<T> &a) -> Matrix3x3<T> { return Matrix3x3<T>(-a._m); }
template<typename T>
auto operator+(const Matrix3x3<T> &a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m + b._m); }
template<typename T>
auto operator+(const Matrix3x3<T> &a, T b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m + b); }
template<typename T>
auto operator+(T a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a + b._m); }
template<typename T>
auto operator-(const Matrix3x3<T> &a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m - b._m); }
template<typename T>
auto operator-(const Matrix3x3<T> &a, T b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m - b); }
template<typename T>
auto operator-(T a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a - b._m); }
template <typename T>
auto operator*(const Matrix3x3<T>& a, T b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m * b); }
template <typename T>
auto operator*(T a, const Matrix3x3<T>& b) -> Matrix3x3<T> { return Matrix3x3<T>(a * b._m); }
template <typename T>
auto operator*(const Matrix3x3<T>& a, const Vector3<T>& b) -> Vector3<T> { return Vector3<T>(a._m * b._v); }
template <typename T>
auto operator*(const Matrix3x3<T>& a, const Matrix3x3<T>& b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m * b._m); }
template<typename T>
auto operator/(const Matrix3x3<T> &a, T b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m / b); }
template<typename T>
auto operator/(T a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a / b._m); }

#ifdef HINAPE_EIGEN
#include "base/impl/matrix_impl_eigen.h"
#else
#endif
}
#endif //HINAPE_MATRIX_H
