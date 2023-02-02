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
	auto matrix3x3() const -> Matrix3x3<T>;
	auto inverse() const -> Quaternion<T>;

public:
#ifdef HINA_EIGEN
	Eigen::Quaternion<T, Eigen::DontAlign> _q;
	constexpr explicit Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_);
	constexpr explicit Quaternion(T w_, T x_, T y_, T z_);
	constexpr explicit Quaternion(Vector3<T> axis, T angle);
	constexpr explicit Quaternion() = default;
#else
	T w, x, y, z;
#endif
};
template<typename T>
constexpr Quaternion<T>::Quaternion(Vector3<T> axis, T angle) : _q(angle, axis._v.x(), axis._v.y(), axis._v.z()) {}
template<typename T>
constexpr Quaternion<T>::Quaternion(T w_, T x_, T y_, T z_) : _q(w_, x_, y_, z_) {}

template<typename T>
auto operator*(const Quaternion<T> &a, Quaternion<T> &b) -> Quaternion<T> { return Quaternion<T>(a._q * b._q); }
template<typename T>
auto operator*(const Quaternion<T> &a, Vector3<T> &b) -> Vector3<T> { return Vector3<T>(a._q * b._v); }
template<typename T>
auto operator*(const Quaternion<T> &a, T b) -> Quaternion<T> { return Quaternion<T>(a._q * b); }
template<typename T>
auto operator*(T a, const Quaternion<T> &b) -> Quaternion<T> { return Quaternion<T>(a * b._q); }
template<typename T>
auto operator+(const Quaternion<T> &a, const Quaternion<T> &b) -> Quaternion<T> { return Quaternion<T>(a._q + b._q); }
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
