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
private:
#ifdef HINAPE_EIGEN
	Eigen::Matrix<T, 3,3, Eigen::DontAlign> _m;
	constexpr explicit Matrix3x3(Eigen::Matrix<T, 3,3, Eigen::DontAlign> m_);
#else
	T w, x, y, z;
#endif
};
#ifdef HINAPE_EIGEN
#include "base/impl/matrix_impl_eigen.h"
#else
#endif
}
#endif //HINAPE_MATRIX_H
