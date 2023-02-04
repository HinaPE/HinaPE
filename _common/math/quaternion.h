#ifndef HINAPE_QUATERNION_H
#define HINAPE_QUATERNION_H

#ifdef HINA_EIGEN
#include "Eigen/Eigen"
#endif
#include <type_traits>

#include "math/matrix.h"

namespace HinaPE::Math
{
template<typename T>
class Quaternion final
{
public:
	auto normalized() -> Quaternion<T>;
	void normalize();
	void rotate(T angle);
	auto axis()	const -> Vector3<T>;
	auto angle() const -> T;
	void get_axis_and_angle(Vector3<T> *axis, T *angle) const;
	auto inverse() const -> Quaternion<T>;
	auto matrix3x3() const -> Matrix3x3<T>;
	auto matrix4x4() const -> Matrix4x4<T>;
	auto euler() const -> Vector3<T>;

public:
#ifdef HINA_EIGEN
	Eigen::Quaternion<T, Eigen::DontAlign> _q;
	template<typename U>
	constexpr Quaternion(const std::initializer_list<U> &lst);
	constexpr explicit Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_);
	constexpr explicit Quaternion() = default;
	constexpr explicit Quaternion(T w_, T x_, T y_, T z_);
	constexpr explicit Quaternion(T roll, T pitch, T yaw);
	constexpr explicit Quaternion(const Vector3<T> &axis, T angle);
	constexpr explicit Quaternion(const Vector3<T>& from, const Vector3<T>& to);
	constexpr explicit Quaternion(const Matrix3x3<T> &m);
#else
	T w, x, y, z;
#endif
};

template<typename T>
auto operator*(const Quaternion<T> &q, const Vector3<T> &v) -> Vector3<T> { return {q._q * v._v}; }
template<typename T>
auto operator*(const Quaternion<T> &a, const Quaternion<T> &b) -> Quaternion<T> { return {a._q * b._q}; }
template<typename T>
auto to_radians(T v) -> T { return v * (static_cast<T>(HINA_PI) / 180.0f); }
template<typename T>
auto to_degrees(T v) -> T { return v * (180.0f / static_cast<T>(HINA_PI)); }
}
#ifdef HINA_EIGEN
#include "impl/quaternion_impl_eigen.h"
#else
#endif

#ifdef HINAPE_DOUBLE
using mQuaternion = HinaPE::Math::Quaternion<double>;
#else
using mQuaternion = HinaPE::Math::Quaternion<float>;
#endif
#endif //HINAPE_QUATERNION_H
