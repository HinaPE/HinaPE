#ifndef HINAPE_MATRIX_H
#define HINAPE_MATRIX_H

#include "math/math_ext.h"

#ifdef HINA_EIGEN
#include "Eigen/Eigen"
#endif
#include <type_traits>

namespace HinaPE::Math
{
// ============================== Matrix3x3 ==============================
template<typename T>
class Matrix3x3 final
{
public:
	inline void inverse() { _m = _m.inverse(); }
	inline void transpose() { _m = _m.transpose(); }
	inline auto inversed() const -> Matrix3x3 { return Matrix3x3(_m.inverse()); }
	inline auto transposed() const -> Matrix3x3 { return Matrix3x3(_m.transpose()); }
	inline auto data() const -> const T * { return _m.data(); }
	inline auto frobenius_norm() const -> T { return _m.norm(); }
	inline auto as_float() const -> Matrix3x3<float> { return Matrix3x3<float>(_m.template cast<float>()); }

public:
	static inline constexpr auto Zero() -> Matrix3x3 { return Matrix3x3(Eigen::Matrix<T, 3, 3, Eigen::DontAlign>::Zero()); }
	static inline constexpr auto Identity() -> Matrix3x3 { return Matrix3x3(Eigen::Matrix<T, 3, 3, Eigen::DontAlign>::Identity()); }

public:
	auto operator[](size_t i) -> T & { return _m(i); }
	auto operator[](size_t i) const -> const T & { return _m(i); }
	auto operator()(size_t i, size_t j) -> T & { return _m(i, j); }
	auto operator()(size_t i, size_t j) const -> const T & { return _m(i, j); }

public:
#ifdef HINA_EIGEN
	template<typename U>
	Matrix3x3(const std::initializer_list<U> &lst);
	constexpr explicit Matrix3x3();
	constexpr explicit Matrix3x3(Eigen::Matrix<T, 3, 3, Eigen::DontAlign> m_);
	constexpr Matrix3x3(const Matrix3x3<T> &m_) = default;
	constexpr Matrix3x3(Matrix3x3<T> &&m_) noexcept = default;
	constexpr auto operator=(const Matrix3x3<T> &m_) -> Matrix3x3<T> & = default;
	constexpr auto operator=(Matrix3x3<T> &&m_) noexcept -> Matrix3x3<T> & = default;
	~Matrix3x3() = default;
	Eigen::Matrix<T, 3, 3, Eigen::DontAlign> _m;
#endif
};

//@formatter:off
template<typename T> auto operator+(const Matrix3x3<T> &a) -> Matrix3x3<T> { return Matrix3x3<T>(a._m); }
template<typename T> auto operator-(const Matrix3x3<T> &a) -> Matrix3x3<T> { return Matrix3x3<T>(-a._m); }
template<typename T> auto operator+(const Matrix3x3<T> &a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m + b._m); }
template<typename T> auto operator+(const Matrix3x3<T> &a, T b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m + b); }
template<typename T> auto operator+(T a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a + b._m); }
template<typename T> auto operator-(const Matrix3x3<T> &a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m - b._m); }
template<typename T> auto operator-(const Matrix3x3<T> &a, T b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m - b); }
template<typename T> auto operator-(T a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a - b._m); }
template<typename T> auto operator*(const Matrix3x3<T>& a, T b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m * b); }
template<typename T> auto operator*(T a, const Matrix3x3<T>& b) -> Matrix3x3<T> { return Matrix3x3<T>(a * b._m); }
template<typename T> auto operator*(const Matrix3x3<T>& a, const Vector3<T>& b) -> Vector3<T> { return Vector3<T>(a._m * b._v); }
template<typename T> auto operator*(const Matrix3x3<T>& a, const Matrix3x3<T>& b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m * b._m); }
template<typename T> auto operator/(const Matrix3x3<T> &a, T b) -> Matrix3x3<T> { return Matrix3x3<T>(a._m / b); }
template<typename T> auto operator/(T a, const Matrix3x3<T> &b) -> Matrix3x3<T> { return Matrix3x3<T>(a / b._m); }
//@formatter:on


// ============================== Matrix4x4 ==============================
template<typename T>
class Matrix4x4 final
{
public:
	inline void inverse() { _m = _m.inverse(); }
	inline void transpose() { _m = _m.transpose(); }
	inline auto inversed() const -> Matrix4x4 { return Matrix4x4(_m.inverse()); }
	inline auto transposed() const -> Matrix4x4 { return Matrix4x4(_m.transpose()); }
	inline auto data() const -> const T * { return _m.data(); }
	inline auto frobenius_norm() const -> T { return _m.norm(); }
	inline auto as_float() const -> Matrix4x4<float> { return Matrix4x4<float>(_m.template cast<float>()); }

public:
	static inline constexpr auto Zero() -> Matrix4x4 { return Matrix4x4(Eigen::Matrix<T, 4, 4, Eigen::DontAlign>::Zero()); }
	static inline constexpr auto Identity() -> Matrix4x4 { return Matrix4x4(Eigen::Matrix<T, 4, 4, Eigen::DontAlign>::Identity()); }
	static inline constexpr auto make_translation_matrix(const Vector3<T> &t) -> Matrix4x4<T>
	{
		return Matrix4x4(
				Eigen::Matrix<T, 4, 4, Eigen::DontAlign>{{1, 0, 0, t.x()},
														 {0, 1, 0, t.y()},
														 {0, 0, 1, t.z()},
														 {0, 0, 0, 1}});
	}
	static inline constexpr auto make_rotation_matrix(const Vector3<T> &axis, T angle) -> Matrix4x4<T> { return Matrix4x4(Eigen::AngleAxis<T>(angle, axis._v)); } // TODO
	static inline constexpr auto make_scale_matrix(const Vector3<T> &s) -> Matrix4x4<T>
	{
		return Matrix4x4(
				Eigen::Matrix<T, 4, 4, Eigen::DontAlign>{{s.x(), 0,     0,     0},
														 {0,     s.y(), 0,     0},
														 {0,     0,     s.z(), 0},
														 {0,     0,     0,     1}});
	}
public:
	auto operator[](size_t i) -> T & { return _m(i); }
	auto operator[](size_t i) const -> const T & { return _m(i); }
	auto operator()(size_t i, size_t j) -> T & { return _m(i, j); }
	auto operator()(size_t i, size_t j) const -> const T & { return _m(i, j); }

public:
#ifdef HINA_EIGEN
	template<typename U>
	Matrix4x4(const std::initializer_list<U> &lst);
	constexpr explicit Matrix4x4();
	constexpr explicit Matrix4x4(Eigen::Matrix<T, 4, 4, Eigen::DontAlign> m_);
	constexpr Matrix4x4(const Matrix4x4<T> &m_) = default;
	constexpr Matrix4x4(Matrix4x4<T> &&m_) noexcept = default;
	constexpr auto operator=(const Matrix4x4<T> &m_) -> Matrix4x4<T> & = default;
	constexpr auto operator=(Matrix4x4<T> &&m_) noexcept -> Matrix4x4<T> & = default;
	~Matrix4x4() = default;
	Eigen::Matrix<T, 4, 4, Eigen::DontAlign> _m;
#endif
};

//@formatter:off
template<typename T> auto operator+(const Matrix4x4<T> &a) -> Matrix4x4<T> { return Matrix4x4<T>(a._m); }
template<typename T> auto operator-(const Matrix4x4<T> &a) -> Matrix4x4<T> { return Matrix4x4<T>(-a._m); }
template<typename T> auto operator+(const Matrix4x4<T> &a, const Matrix4x4<T> &b) -> Matrix4x4<T> { return Matrix4x4<T>(a._m + b._m); }
template<typename T> auto operator+(const Matrix4x4<T> &a, T b) -> Matrix4x4<T> { return Matrix4x4<T>(a._m + b); }
template<typename T> auto operator+(T a, const Matrix4x4<T> &b) -> Matrix4x4<T> { return Matrix4x4<T>(a + b._m); }
template<typename T> auto operator-(const Matrix4x4<T> &a, const Matrix4x4<T> &b) -> Matrix4x4<T> { return Matrix4x4<T>(a._m - b._m); }
template<typename T> auto operator-(const Matrix4x4<T> &a, T b) -> Matrix4x4<T> { return Matrix4x4<T>(a._m - b); }
template<typename T> auto operator-(T a, const Matrix4x4<T> &b) -> Matrix4x4<T> { return Matrix4x4<T>(a - b._m); }
template<typename T> auto operator*(const Matrix4x4<T>& a, T b) -> Matrix4x4<T> { return Matrix4x4<T>(a._m * b); }
template<typename T> auto operator*(T a, const Matrix4x4<T>& b) -> Matrix4x4<T> { return Matrix4x4<T>(a * b._m); }
template<typename T> auto operator*(const Matrix4x4<T>& a, const Vector4<T>& b) -> Vector4<T> { return Vector4<T>(a._m * b._v); }
template<typename T> auto operator*(const Matrix4x4<T>& a, const Matrix4x4<T>& b) -> Matrix4x4<T> { return Matrix4x4<T>(a._m * b._m); }
template<typename T> auto operator/(const Matrix4x4<T> &a, T b) -> Matrix4x4<T> { return Matrix4x4<T>(a._m / b); }
template<typename T> auto operator/(T a, const Matrix4x4<T> &b) -> Matrix4x4<T> { return Matrix4x4<T>(a / b._m); }
//@formatter:on


template<typename T>
auto similar(const Matrix3x3<T> &a, const Matrix3x3<T> &b) -> bool
{
	auto eps = static_cast<T>(1e-2);
	return similar(a._m(0, 0), b._m(0, 0), eps) && similar(a._m(0, 1), b._m(0, 1), eps) && similar(a._m(0, 2), b._m(0, 2), eps) &&
		   similar(a._m(1, 0), b._m(1, 0), eps) && similar(a._m(1, 1), b._m(1, 1), eps) && similar(a._m(1, 2), b._m(1, 2), eps) &&
		   similar(a._m(2, 0), b._m(2, 0), eps) && similar(a._m(2, 1), b._m(2, 1), eps) && similar(a._m(2, 2), b._m(2, 2), eps);
}
template<typename T>
auto similar(const Matrix4x4<T> &a, const Matrix4x4<T> &b) -> bool
{
	auto eps = static_cast<T>(1e-2);
	return similar(a._m(0, 0), b._m(0, 0), eps) && similar(a._m(0, 1), b._m(0, 1), eps) && similar(a._m(0, 2), b._m(0, 2), eps) && similar(a._m(0, 3), b._m(0, 3), eps) &&
		   similar(a._m(1, 0), b._m(1, 0), eps) && similar(a._m(1, 1), b._m(1, 1), eps) && similar(a._m(1, 2), b._m(1, 2), eps) && similar(a._m(1, 3), b._m(1, 3), eps) &&
		   similar(a._m(2, 0), b._m(2, 0), eps) && similar(a._m(2, 1), b._m(2, 1), eps) && similar(a._m(2, 2), b._m(2, 2), eps) && similar(a._m(2, 3), b._m(2, 3), eps) &&
		   similar(a._m(3, 0), b._m(3, 0), eps) && similar(a._m(3, 1), b._m(3, 1), eps) && similar(a._m(3, 2), b._m(3, 2), eps) && similar(a._m(3, 3), b._m(3, 3), eps);
}
}
#ifdef HINA_EIGEN
#include "impl/matrix_impl_eigen.h"
#else
#endif

#ifdef HINAPE_DOUBLE
using mMatrix3x3 = HinaPE::Math::Matrix3x3<double>;
using mMatrix4x4 = HinaPE::Math::Matrix4x4<double>;
#else
using mMatrix3x3 = HinaPE::Math::Matrix3x3<float>;
using mMatrix4x4 = HinaPE::Math::Matrix4x4<float>;
#endif
#endif //HINAPE_MATRIX_H
