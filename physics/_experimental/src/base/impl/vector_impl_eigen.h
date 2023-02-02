#ifndef HINAPE_VECTOR_EIGEN_H
#define HINAPE_VECTOR_EIGEN_H

namespace Hina::Base
{
template<typename T>
template<typename U>
Vector3<T>::Vector3(const std::initializer_list<U> &lst)
{
	assert(lst.size() == 3);

	auto input_elem = lst.begin();
	_v.x() = static_cast<T>(*input_elem);
	_v.y() = static_cast<T>(*(++input_elem));
	_v.z() = static_cast<T>(*(++input_elem));
}

//@formatter:off
template<typename T> constexpr Vector3<T>::Vector3() : _v(0, 0, 0) 											{}
template<typename T> constexpr Vector3<T>::Vector3(T s) : _v(s, s, s) 										{}
template<typename T> constexpr Vector3<T>::Vector3(T x_, T y_, T z_) : _v(x_, y_, z_) 						{}
template<typename T> constexpr Vector3<T>::Vector3(const Vector3 &v) : _v(v._v) 							{}
template<typename T> constexpr Vector3<T>::Vector3(Vector3 &&v) noexcept : _v(std::move(v._v)) 				{}
template<typename T> constexpr Vector3<T>::Vector3(Eigen::Matrix<T, 3, 1, Eigen::DontAlign> v_) : _v(v_) 	{}

template<typename T> auto Vector3<T>::x() -> T & 								{ return this->_v[0]; }
template<typename T> auto Vector3<T>::y() -> T & 								{ return this->_v[1]; }
template<typename T> auto Vector3<T>::z() -> T & 								{ return this->_v[2]; }
template<typename T> auto Vector3<T>::x() const -> const T & 					{ return this->_v[0]; }
template<typename T> auto Vector3<T>::y() const -> const T & 					{ return this->_v[1]; }
template<typename T> auto Vector3<T>::z() const -> const T & 					{ return this->_v[2]; }
template<typename T> auto Vector3<T>::at(size_t i) -> T & 						{ return this->_v[i]; }
template<typename T> auto Vector3<T>::at(size_t i) const -> const T & 			{ return this->_v[i]; }
template<typename T> auto Vector3<T>::sum() const -> T 							{ return x() + y() + z(); }
template<typename T> auto Vector3<T>::avg() const -> T 							{ return sum() / 3; }
template<typename T> auto Vector3<T>::min() const -> T 							{ return std::min(x(), std::min(y(), z())); }
template<typename T> auto Vector3<T>::max() const -> T 							{ return std::max(x(), std::max(y(), z())); }
template<typename T> auto Vector3<T>::dot(const Vector3 &v_) const -> T 		{ return this->_v.dot(v_._v); }
template<typename T> auto Vector3<T>::cross(const Vector3 &v_) const -> Vector3 { return Vector3(this->_v.cross(v_._v)); }
template<typename T> auto Vector3<T>::length() -> T 							{ return _v.norm(); }
template<typename T> auto Vector3<T>::length_squared() -> T 					{ return _v.squaredNorm(); }
template<typename T> void Vector3<T>::normalize() 								{ _v.normalize(); }
//@formatter:on

template<typename T>
template<typename U>
auto Vector3<T>::operator=(const std::initializer_list<U> &lst) -> Vector3 &
{
	assert(lst.size() >= 3);

	auto input_elem = lst.begin();
	_v.x = static_cast<T>(*input_elem);
	_v.y = static_cast<T>(*(++input_elem));
	_v.z = static_cast<T>(*(++input_elem));
}
template<typename T>
auto Vector3<T>::operator=(const Vector3 &v_) -> Vector3 &
{
	this->_v = v_._v;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator[](size_t i) -> T &
{
	assert(i < 3);
	return this->_v[i];
}
template<typename T>
auto Vector3<T>::operator[](size_t i) const -> const T &
{
	assert(i < 3);
	return this->_v[i];
}
template<typename T>
auto Vector3<T>::operator+=(T v_) -> Vector3 &
{
	this->_v.x() += v_;
	this->_v.y() += v_;
	this->_v.z() += v_;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator+=(const Vector3 &v_) -> Vector3 &
{
	this->_v.x() += v_.x();
	this->_v.y() += v_.y();
	this->_v.z() += v_.z();
	return (*this);
}
template<typename T>
auto Vector3<T>::operator-=(T v_) -> Vector3 &
{
	this->_v.x() -= v_;
	this->_v.y() -= v_;
	this->_v.z() -= v_;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator-=(const Vector3 &v_) -> Vector3 &
{
	this->_v.x() -= v_.x();
	this->_v.y() -= v_.y();
	this->_v.z() -= v_.z();
	return (*this);
}
template<typename T>
auto Vector3<T>::operator*=(T v_) -> Vector3 &
{
	this->_v.x() *= v_;
	this->_v.y() *= v_;
	this->_v.z() *= v_;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator*=(const Vector3 &v_) -> Vector3 &
{
	this->_v.x() *= v_.x();
	this->_v.y() *= v_.y();
	this->_v.z() *= v_.z();
	return (*this);
}
template<typename T>
auto Vector3<T>::operator/=(T v_) -> Vector3 &
{
	this->_v.x() /= v_;
	this->_v.y() /= v_;
	this->_v.z() /= v_;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator/=(const Vector3 &v_) -> Vector3 &
{
	this->_v.x() /= v_.x();
	this->_v.y() /= v_.y();
	this->_v.z() /= v_.z();
	return (*this);
}
template<typename T>
auto Vector3<T>::operator==(const Vector3 &v_) const -> bool { return this->_v == v_.v; }
template<typename T>
auto Vector3<T>::operator!=(const Vector3 &v_) const -> bool { return this->_v != v_.v; }
}
#endif //HINAPE_VECTOR_EIGEN_H