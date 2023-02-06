#ifndef HINAPE_MATRIX_IMPL_EIGEN_H
#define HINAPE_MATRIX_IMPL_EIGEN_H

#include "../matrix.h"

namespace HinaPE::Math
{
// ============================== Matrix3x3 ==============================
template<typename T>
constexpr Matrix3x3<T>::Matrix3x3(Eigen::Matrix<T, 3, 3, Eigen::DontAlign> m_) : _m(m_) {}

template<typename T>
template<typename U>
Matrix3x3<T>::Matrix3x3(const std::initializer_list<U> &lst)
{
	assert(lst.size() == 9);

	auto input_elem = lst.begin();
	_m(0, 0) = static_cast<T>(*input_elem);
	_m(0, 1) = static_cast<T>(*(++input_elem));
	_m(0, 2) = static_cast<T>(*(++input_elem));
	_m(1, 0) = static_cast<T>(*(++input_elem));
	_m(1, 1) = static_cast<T>(*(++input_elem));
	_m(1, 2) = static_cast<T>(*(++input_elem));
	_m(2, 0) = static_cast<T>(*(++input_elem));
	_m(2, 1) = static_cast<T>(*(++input_elem));
	_m(2, 2) = static_cast<T>(*(++input_elem));
}

template<typename T>
constexpr Matrix3x3<T>::Matrix3x3()
{
	_m = Eigen::Matrix<T, 3, 3, Eigen::DontAlign>::Zero();
}

// ============================== Matrix4x4 ==============================
template<typename T>
constexpr Matrix4x4<T>::Matrix4x4(Eigen::Matrix<T, 4, 4, Eigen::DontAlign> m_) : _m(m_) {}

template<typename T>
template<typename U>
Matrix4x4<T>::Matrix4x4(const std::initializer_list<U> &lst)
{
	assert(lst.size() == 16);

	auto input_elem = lst.begin();
	_m(0, 0) = static_cast<T>(*input_elem);
	_m(0, 1) = static_cast<T>(*(++input_elem));
	_m(0, 2) = static_cast<T>(*(++input_elem));
	_m(0, 3) = static_cast<T>(*(++input_elem));
	_m(1, 0) = static_cast<T>(*(++input_elem));
	_m(1, 1) = static_cast<T>(*(++input_elem));
	_m(1, 2) = static_cast<T>(*(++input_elem));
	_m(1, 3) = static_cast<T>(*(++input_elem));
	_m(2, 0) = static_cast<T>(*(++input_elem));
	_m(2, 1) = static_cast<T>(*(++input_elem));
	_m(2, 2) = static_cast<T>(*(++input_elem));
	_m(2, 3) = static_cast<T>(*(++input_elem));
	_m(3, 0) = static_cast<T>(*(++input_elem));
	_m(3, 1) = static_cast<T>(*(++input_elem));
	_m(3, 2) = static_cast<T>(*(++input_elem));
	_m(3, 3) = static_cast<T>(*(++input_elem));
}

template<typename T>
constexpr Matrix4x4<T>::Matrix4x4()
{
	_m = Eigen::Matrix<T, 4, 4, Eigen::DontAlign>::Zero();
}
}
#endif //HINAPE_MATRIX_IMPL_EIGEN_H
