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
	auto euler() const -> Vector3<T>;

public:
#ifdef HINA_EIGEN
	Eigen::Quaternion<T, Eigen::DontAlign> _q;
	constexpr explicit Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_);
	constexpr explicit Quaternion() = default;
#else
	T w, x, y, z;
#endif
};

template<typename T>
auto operator*(const Quaternion<T> &q, const Vector3<T> &v) -> Vector3<T> { return {q._q * v._v}; }
template<typename T>
auto operator*(const Quaternion<T> &a, const Quaternion<T> &b) -> Quaternion<T> { return {a._q * b._q}; }
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
