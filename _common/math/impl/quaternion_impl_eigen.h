#ifndef HINAPE_QUATERNION_IMPL_EIGEN_H
#define HINAPE_QUATERNION_IMPL_EIGEN_H

#include "../quaternion.h"

namespace HinaPE::Math
{
template<typename T>
Quaternion<T>::Quaternion() : _q(1, 0, 0, 0) { _q.normalize(); }
template<typename T>
Quaternion<T>::Quaternion(T w_, T x_, T y_, T z_) : _q(w_, x_, y_, z_) { _q.normalize(); }
template<typename T>
Quaternion<T>::Quaternion(Eigen::Quaternion<T, Eigen::DontAlign> q_) : _q(q_) { _q.normalize(); }
template<typename T>
Quaternion<T>::Quaternion(T roll, T pitch, T yaw)
{
	// ref: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Euler_angles_to_quaternion_conversion
	auto radians_roll = to_radians(roll);
	auto radians_pitch = to_radians(pitch);
	auto radians_yaw = to_radians(yaw);

	double cr = std::cos(radians_roll * static_cast<T>(0.5));
	double sr = std::sin(radians_roll * static_cast<T>(0.5));
	double cp = std::cos(radians_pitch * static_cast<T>(0.5));
	double sp = std::sin(radians_pitch * static_cast<T>(0.5));
	double cy = std::cos(radians_yaw * static_cast<T>(0.5));
	double sy = std::sin(radians_yaw * static_cast<T>(0.5));

	_q.w() = cr * cp * cy + sr * sp * sy;
	_q.x() = sr * cp * cy - cr * sp * sy;
	_q.y() = cr * sp * cy + sr * cp * sy;
	_q.z() = cr * cp * sy - sr * sp * cy;

	_q.normalize();
}
template<typename T>
Quaternion<T>::Quaternion(const std::initializer_list<T> &lst)
{
	assert(lst.size() == 4);

	auto input_elem = lst.begin();
	_q.w() = static_cast<T>((*input_elem));
	_q.x() = static_cast<T>((*++input_elem));
	_q.y() = static_cast<T>((*++input_elem));
	_q.z() = static_cast<T>((*++input_elem));

	_q.normalize();
}
template<typename T>
auto Quaternion<T>::operator=(const std::initializer_list<T> &lst) -> Quaternion<T> &
{
	assert(lst.size() == 4);

	auto input_elem = lst.begin();
	_q.w() = static_cast<T>((*input_elem));
	_q.x() = static_cast<T>((*++input_elem));
	_q.y() = static_cast<T>((*++input_elem));
	_q.z() = static_cast<T>((*++input_elem));

	_q.normalize();
}
template<typename T>
Quaternion<T>::Quaternion(const Vector3<T> &axis, T angle) : _q(Eigen::AngleAxis<real>(angle * 2, axis._v.normalized()))
{
//	Vector3<T> normalized_axis = axis.normalized();
//	T s = std::sin(angle / 2);
//
//	_q.x() = normalized_axis.x() * s;
//	_q.y() = normalized_axis.y() * s;
//	_q.z() = normalized_axis.z() * s;
//	_q.w() = std::cos(angle / 2);

	assert(false); // NOTE: not tested

	_q.normalize();
}
template<typename T>
Quaternion<T>::Quaternion(const Matrix3x3<T> &m)
{
	const T quater = static_cast<T>(0.25);

	T w, x, y, z;

	T onePlusTrace = m.trace() + 1;

	if (onePlusTrace > std::numeric_limits<T>::epsilon())
	{
		T S = std::sqrt(onePlusTrace) * 2;
		w = quater * S;
		x = (m(2, 1) - m(1, 2)) / S;
		y = (m(0, 2) - m(2, 0)) / S;
		z = (m(1, 0) - m(0, 1)) / S;
	} else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
	{
		T S = std::sqrt(1 + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
		w = (m(2, 1) - m(1, 2)) / S;
		x = quater * S;
		y = (m(0, 1) + m(1, 0)) / S;
		z = (m(0, 2) + m(2, 0)) / S;
	} else if (m(1, 1) > m(2, 2))
	{
		T S = std::sqrt(1 + m(1, 1) - m(0, 0) - m(2, 2)) * 2;
		w = (m(0, 2) - m(2, 0)) / S;
		x = (m(0, 1) + m(1, 0)) / S;
		y = quater * S;
		z = (m(1, 2) + m(2, 1)) / S;
	} else
	{
		T S = std::sqrt(1 + m(2, 2) - m(0, 0) - m(1, 1)) * 2;
		w = (m(1, 0) - m(0, 1)) / S;
		x = (m(0, 2) + m(2, 0)) / S;
		y = (m(1, 2) + m(2, 1)) / S;
		z = quater * S;
	}

	_q.x() = x;
	_q.y() = y;
	_q.z() = z;
	_q.w() = w;

	_q.normalize();
}
template<typename T>
Quaternion<T>::Quaternion(const Vector3<T> &from, const Vector3<T> &to)
{
	_q.setFromTwoVectors(from._v, to._v);
	_q.normalize();
}
template<typename T>
Quaternion<T>::Quaternion(const Quaternion<T> &q_) : _q(q_._q) {}
template<typename T>
auto Quaternion<T>::operator=(const Quaternion<T> &q_) -> Quaternion<T> &
{
	_q = q_._q;
	return *this;
}
template<typename T>
Quaternion<T>::Quaternion(Quaternion<T> &&q_) noexcept : _q(std::move(q_._q)) {}
template<typename T>
auto Quaternion<T>::operator=(Quaternion<T> &&q_) noexcept -> Quaternion<T> &
{
	_q = std::move(q_._q);
	return *this;
}

template<typename T>
auto Quaternion<T>::normalized() -> Quaternion<T>
{
	return Quaternion<T>(_q.normalized());
}
template<typename T>
void Quaternion<T>::normalize()
{
	_q.normalize();
}
template<typename T>
void Quaternion<T>::rotate(T angle)
{
	Vector3<T> axis = Vector3<T>::Zero();
	T current_angle = Constant::Zero;

	get_axis_and_angle(&axis, &current_angle);

	current_angle += angle;

	auto q_ = Quaternion<T>(axis, current_angle);

	_q.x() = q_._q.x();
	_q.y() = q_._q.y();
	_q.z() = q_._q.z();
	_q.w() = q_._q.w();
}
template<typename T>
auto Quaternion<T>::axis() const -> Vector3<T>
{
	Vector3<T> res(_q.x(), _q.y(), _q.z());
	res.normalize();

	if (2 * std::acos(_q.w()) > Constant::PI)
		return res;
	else
		return -res;
}
template<typename T>
auto Quaternion<T>::angle() const -> T
{
	T res = 2 * std::acos(_q.w());

	if (res < Constant::PI)
		return res;
	else
		return 2 * Constant::PI - res;
}
template<typename T>
void Quaternion<T>::get_axis_and_angle(Vector3<T> *axis, T *angle) const
{
	axis->x() = _q.x();
	axis->y() = _q.y();
	axis->z() = _q.z();
	axis->normalize();

	*angle = 2 * std::acos(_q.w());

	if (*angle > Constant::PI)
	{
		*axis = -*axis;
		*angle = 2 * Constant::PI - *angle;
	}
}
template<typename T>
auto Quaternion<T>::inverse() const -> Quaternion<T> { return Quaternion<T>(_q.inverse()); }
template<typename T>
auto Quaternion<T>::matrix3x3() const -> Matrix3x3<T>
{
	T x = _q.x();
	T y = _q.y();
	T z = _q.z();
	T w = _q.w();

	T _2xx = 2 * x * x;
	T _2yy = 2 * y * y;
	T _2zz = 2 * z * z;
	T _2xy = 2 * x * y;
	T _2xz = 2 * x * z;
	T _2xw = 2 * x * w;
	T _2yz = 2 * y * z;
	T _2yw = 2 * y * w;
	T _2zw = 2 * z * w;

	Matrix3x3<T> m({1 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw,
					_2xy + _2zw, 1 - _2zz - _2xx, _2yz - _2xw,
					_2xz - _2yw, _2yz + _2xw, 1 - _2yy - _2xx});

	return m;
}
template<typename T>
auto Quaternion<T>::matrix4x4() const -> Matrix4x4<T>
{
	T x = _q.x();
	T y = _q.y();
	T z = _q.z();
	T w = _q.w();

	T _2xx = 2 * x * x;
	T _2yy = 2 * y * y;
	T _2zz = 2 * z * z;
	T _2xy = 2 * x * y;
	T _2xz = 2 * x * z;
	T _2xw = 2 * x * w;
	T _2yz = 2 * y * z;
	T _2yw = 2 * y * w;
	T _2zw = 2 * z * w;

	Matrix4x4<T> m({1 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw, Constant::Zero,
					_2xy + _2zw, 1 - _2zz - _2xx, _2yz - _2xw, Constant::Zero,
					_2xz - _2yw, _2yz + _2xw, 1 - _2yy - _2xx, Constant::Zero,
					Constant::Zero, Constant::Zero, Constant::Zero, Constant::One});

	return m;
}
template<typename T>
auto Quaternion<T>::euler() const -> Vector3<T>
{
	T x = _q.x();
	T y = _q.y();
	T z = _q.z();
	T w = _q.w();

	// ref: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code_2
	T roll = std::atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
	T pitch = std::asin(2 * (w * y - z * x));
	T yaw = std::atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));

	return Vector3<T>(to_degrees(roll), to_degrees(pitch), to_degrees(yaw));
}
}
#endif //HINAPE_QUATERNION_IMPL_EIGEN_H
