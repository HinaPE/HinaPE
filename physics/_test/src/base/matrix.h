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
#ifdef HINAPE_EIGEN
	Eigen::Matrix<T, 3,3, Eigen::DontAlign> _m;
	constexpr explicit Matrix3x3(Eigen::Matrix<T, 3,3, Eigen::DontAlign> m_);
#else
	T w, x, y, z;
#endif
};

template <typename T>
auto operator*(const Matrix3x3<T>& a, T b) -> Matrix3x3<T> {
	return Matrix3x3<T>(a._m * b);
}
template <typename T>
auto operator*(T a, const Matrix3x3<T>& b) -> Matrix3x3<T>{
	return Matrix3x3<T>(a * b._m);}
template <typename T>
auto operator*(const Matrix3x3<T>& a, const Vector3<T>& b) -> Vector3<T>{
	return Vector3<T>(a._m * b._v);}
template <typename T>
auto operator*(const Matrix3x3<T>& a, const Matrix3x3<T>& b) -> Matrix3x3<T>{
	return Matrix3x3<T>(a._m * b._m);}

#ifdef HINAPE_EIGEN
#include "base/impl/matrix_impl_eigen.h"
#else
#endif
}
#endif //HINAPE_MATRIX_H
