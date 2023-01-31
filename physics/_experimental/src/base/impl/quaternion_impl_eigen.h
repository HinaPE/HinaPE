#ifndef HINAPE_QUATERNION_IMPL_EIGEN_H
#define HINAPE_QUATERNION_IMPL_EIGEN_H

namespace Hina::Base
{
template<typename T>
constexpr Quaternion<T>::Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_)
{
}

template<typename T>
auto Quaternion<T>::matrix3x3() const -> Matrix3x3<T> { return Matrix3x3<T>(_q.toRotationMatrix()); }
template<typename T>
auto Quaternion<T>::inverse() const -> Quaternion<T> { return Quaternion<T>(_q.inverse()); }
}
#endif //HINAPE_QUATERNION_IMPL_EIGEN_H
