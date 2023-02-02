#ifndef HINAPE_QUATERNION_IMPL_EIGEN_H
#define HINAPE_QUATERNION_IMPL_EIGEN_H

#include "../quaternion.h"

namespace HinaPE::Math
{
template<typename T>
constexpr Quaternion<T>::Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_) : _q(q_) {}

template<typename T>
auto Quaternion<T>::matrix3x3() const -> Matrix3x3<T> { return Matrix3x3<T>(_q.toRotationMatrix()); }
template<typename T>
auto Quaternion<T>::inverse() const -> Quaternion<T> { return Quaternion<T>(_q.inverse()); }
template<typename T>
auto Quaternion<T>::euler() const -> Vector3<T>
{
	return Vector3<T>();
}
}
#endif //HINAPE_QUATERNION_IMPL_EIGEN_H
