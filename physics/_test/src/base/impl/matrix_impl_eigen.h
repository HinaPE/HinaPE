#ifndef HINAPE_MATRIX_IMPL_EIGEN_H
#define HINAPE_MATRIX_IMPL_EIGEN_H

template<typename T>
constexpr Matrix3x3<T>::Matrix3x3(Eigen::Matrix<T, 3, 3, Eigen::DontAlign> m_) : _m(m_) {}

#endif //HINAPE_MATRIX_IMPL_EIGEN_H
