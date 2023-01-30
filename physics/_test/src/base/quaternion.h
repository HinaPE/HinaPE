#ifndef HINAPE_QUATERNION_H
#define HINAPE_QUATERNION_H

#ifdef HINAPE_EIGEN
#include "Eigen/Eigen"
#endif
#include <type_traits>

#include "base/matrix.h"

namespace Hina::Base
{
template<typename T>
class Quaternion final
{
public:
	auto matrix3x3() const -> Matrix3x3<T>;
	auto inverse() const -> Quaternion<T>;

private:
#ifdef HINAPE_EIGEN
	Eigen::Quaternion<T, Eigen::DontAlign> _q;
	constexpr explicit Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_);
#else
	T w, x, y, z;
#endif
};

#ifdef HINAPE_EIGEN
#include "base/impl/quaternion_impl_eigen.h"
#else
#endif
}
#endif //HINAPE_QUATERNION_H
