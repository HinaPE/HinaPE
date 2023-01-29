#ifndef HINAPE_VECTOR_EIGEN_H
#define HINAPE_VECTOR_EIGEN_H

template<typename T>
template<typename U>
Vector3<T>::Vector3(const std::initializer_list<U> &lst)
{
	assert(lst.size() >= 3);

	auto input_elem = lst.begin();
	v.x = static_cast<T>(*input_elem);
	v.y = static_cast<T>(*(++input_elem));
	v.z = static_cast<T>(*(++input_elem));
}

template<typename T>
constexpr Vector3<T>::Vector3() : v(0, 0, 0) {}
template<typename T>
constexpr Vector3<T>::Vector3(T s) : v(s, s, s) {}
template<typename T>
constexpr Vector3<T>::Vector3(T x_, T y_, T z_) : v(x_, y_, z_) {}
template<typename T>
constexpr Vector3<T>::Vector3(const Vector3 &v) : v(v.v) {}
template<typename T>
constexpr Vector3<T>::Vector3(Vector3 &&v) noexcept : v(std::move(v.v)) {}
template<typename T>
constexpr Vector3<T>::Vector3(Eigen::Matrix<T, 3, 1> v_) : v(v_) {}

template<typename T>
T Vector3<T>::length() { return v.norm(); }
template<typename T>
void Vector3<T>::normalize() { v.normalize(); }

template<typename T>
auto Vector3<T>::x() -> T & { return this->v[0]; }
template<typename T>
auto Vector3<T>::y() -> T & { return this->v[1]; }
template<typename T>
auto Vector3<T>::z() -> T & { return this->v[2]; }
template<typename T>
auto Vector3<T>::x() const -> const T & { return this->v[0]; }
template<typename T>
auto Vector3<T>::y() const -> const T & { return this->v[1]; }
template<typename T>
auto Vector3<T>::z() const -> const T & { return this->v[2]; }
template<typename T>
auto Vector3<T>::at(size_t i) -> T &
{
	assert(i < 3);
	return this->v[i];
}
template<typename T>
auto Vector3<T>::at(size_t i) const -> const T &
{
	assert(i < 3);
	return this->v[i];
}
template<typename T>
auto Vector3<T>::dot(const Vector3 &v_) const -> T { return this->v.dot(v_.v); }
template<typename T>
auto Vector3<T>::cross(const Vector3 &v_) const -> Vector3 { return Vector3(this->v.cross(v_.v)); }

template<typename T>
template<typename U>
auto Vector3<T>::operator=(const std::initializer_list<U> &lst) -> Vector3 &
{
	assert(lst.size() >= 3);

	auto input_elem = lst.begin();
	v.x = static_cast<T>(*input_elem);
	v.y = static_cast<T>(*(++input_elem));
	v.z = static_cast<T>(*(++input_elem));
}
template<typename T>
auto Vector3<T>::operator=(const Vector3 &v_) -> Vector3 &
{
	this->v = v_.v;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator[](size_t i) -> T &
{
	assert(i < 3);
	return this->v[i];
}
template<typename T>
auto Vector3<T>::operator[](size_t i) const -> const T &
{
	assert(i < 3);
	return this->v[i];
}
template<typename T>
auto Vector3<T>::operator+=(T v_) -> Vector3 &
{
	this->v.x() += v_;
	this->v.y() += v_;
	this->v.z() += v_;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator+=(const Vector3 &v_) -> Vector3 &
{
	this->v.x() += v_.x();
	this->v.y() += v_.y();
	this->v.z() += v_.z();
	return (*this);
}
template<typename T>
auto Vector3<T>::operator-=(T v_) -> Vector3 &
{
	this->v.x() -= v_;
	this->v.y() -= v_;
	this->v.z() -= v_;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator-=(const Vector3 &v_) -> Vector3 &
{
	this->v.x() -= v_.x();
	this->v.y() -= v_.y();
	this->v.z() -= v_.z();
	return (*this);
}
template<typename T>
auto Vector3<T>::operator*=(T v_) -> Vector3 &
{
	this->v.x() *= v_;
	this->v.y() *= v_;
	this->v.z() *= v_;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator*=(const Vector3 &v_) -> Vector3 &
{
	this->v.x() *= v_.x();
	this->v.y() *= v_.y();
	this->v.z() *= v_.z();
	return (*this);
}
template<typename T>
auto Vector3<T>::operator/=(T v_) -> Vector3 &
{
	this->v.x() /= v_;
	this->v.y() /= v_;
	this->v.z() /= v_;
	return (*this);
}
template<typename T>
auto Vector3<T>::operator/=(const Vector3 &v_) -> Vector3 &
{
	this->v.x() /= v_.x();
	this->v.y() /= v_.y();
	this->v.z() /= v_.z();
	return (*this);
}
template<typename T>
auto Vector3<T>::operator==(const Vector3 &v_) const -> bool { return this->v == v_.v; }
template<typename T>
auto Vector3<T>::operator!=(const Vector3 &v_) const -> bool { return this->v != v_.v; }

#endif //HINAPE_VECTOR_EIGEN_H