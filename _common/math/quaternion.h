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
	inline auto x() const -> T { return _q.x(); }
	inline auto y() const -> T { return _q.y(); }
	inline auto z() const -> T { return _q.z(); }
	inline auto w() const -> T { return _q.w(); }

	auto normalized() -> Quaternion<T>;
	void normalize();
	void rotate(T angle);
	auto axis() const -> Vector3<T>;
	auto angle() const -> T;
	void get_axis_and_angle(Vector3<T> *axis, T *angle) const;
	auto inverse() const -> Quaternion<T>;
	auto matrix3x3() const -> Matrix3x3<T>;
	auto matrix4x4() const -> Matrix4x4<T>;
	auto euler() const -> Vector3<T>;

public:
	static inline constexpr auto Identity() -> Quaternion<T> { return Quaternion<T>(1, 0, 0, 0); }

public:
#ifdef HINA_EIGEN
	Eigen::Quaternion<T, Eigen::DontAlign> _q;

	// constructors
	Quaternion();
	Quaternion(T w_, T x_, T y_, T z_);
	Quaternion(T roll, T pitch, T yaw);
	Quaternion(const Vector3<T> &axis, T angle);
	Quaternion(const Vector3<T> &from, const Vector3<T> &to);
	explicit Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_);
	explicit Quaternion(const Matrix3x3<T> &m);

	// initializer list constructor and assignment operator
	Quaternion(const std::initializer_list<T> &lst);
	auto operator=(const std::initializer_list<T> &lst) -> Quaternion<T> &;

	// copy constructor and assignment operator
	Quaternion(const Quaternion<T> &q_); // Quaternion<float> q1(q2)
	auto operator=(const Quaternion<T> &q_) -> Quaternion<T> &; // q1 = q2

	// move constructor and assignment operator
	Quaternion(Quaternion<T> &&q_) noexcept;
	auto operator=(Quaternion<T> &&q_) noexcept -> Quaternion<T> &;

	// equal operator
	auto operator==(const Quaternion<T> &q_) const -> bool; // q1 == q2

	// destructor
	~Quaternion() = default;
#else
	T w, x, y, z;
#endif
};
template<typename T>
auto operator*(const Quaternion<T> &q, const Vector3<T> &v) -> Vector3<T> { return Vector3<T>(q._q * v._v); }
template<typename T>
auto operator*(const Quaternion<T> &a, const Quaternion<T> &b) -> Quaternion<T> { return Quaternion<T>(a._q * b._q); }
template<typename T>
auto to_radians(T v) -> T { return v * (static_cast<T>(HINA_PI) / 180.0f); }
template<typename T>
auto to_degrees(T v) -> T { return v * (180.0f / static_cast<T>(HINA_PI)); }
template<typename T>
auto operator<<(std::ostream &os, const Quaternion<T> &q_) -> std::ostream &
{
	os << "[" << q_.x() << "i, " << q_.y() << "j, " << q_.z() << "k, " << q_.w() << "]";
	return os;
}
template<typename T>
auto Quaternion<T>::operator==(const Quaternion<T> &q_) const -> bool
{
	auto eps = static_cast<T>(1e-2);
	return similar(x(), q_.x(), eps) &&
		   similar(y(), q_.y(), eps) &&
		   similar(z(), q_.z(), eps) &&
		   similar(w(), q_.w(), eps);
}
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
