#ifndef HINAPE_VECTOR_H
#define HINAPE_VECTOR_H

#include <type_traits>

namespace HinaPE::Base
{
template<typename T>
class Vector3 final
{
public:
	T x, y, z;

public:
	inline void normalize();

public:
	template<typename U>
	Vector3(const std::initializer_list<U> &lst);
	constexpr Vector3() : x(0), y(0), z(0) {}
	constexpr Vector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
	constexpr Vector3(const Vector3 &v) = default;
	constexpr Vector3(Vector3 &&v) noexcept = default;
};

template<typename T>
template<typename U>
Vector3<T>::Vector3(const std::initializer_list<U> &lst)
{
	assert(lst.size() >= 3);

	auto input_elem = lst.begin();
	x = static_cast<T>(*input_elem);
	y = static_cast<T>(*(++input_elem));
	z = static_cast<T>(*(++input_elem));
}

template<typename T>
void Vector3<T>::normalize()
{
}
}

#endif //HINAPE_VECTOR_H
