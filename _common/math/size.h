#ifndef HINAPE_SIZE_H
#define HINAPE_SIZE_H

#include "math/math_ext.h"

#include <cstddef>
#include <array>
#include <type_traits>
#include <cassert>

namespace HinaPE::Math
{
class Size3
{
public:
	size_t x, y, z;

public:
	template<typename U>
	Size3(const std::initializer_list<U> &lst);
	constexpr Size3() : x(0), y(0), z(0) {}
	constexpr Size3(size_t x_, size_t y_, size_t z_) : x(x_), y(y_), z(z_) {}
	constexpr Size3(const Size3 &v) = default;
	constexpr Size3(Size3 &&v) noexcept = default;
	auto operator=(const Size3 &v) -> Size3 & = default;
	auto operator=(Size3 &&v) noexcept -> Size3 & = default;
};

template<typename U>
Size3::Size3(const std::initializer_list<U> &lst)
{
	assert(lst.size() >= 3);

	auto input_elem = lst.begin();
	x = static_cast<U>(*input_elem);
	y = static_cast<U>(*(++input_elem));
	z = static_cast<U>(*(++input_elem));
}

inline auto operator+(const Size3 &a) -> Size3 { return a; }
inline auto operator-(const Size3 &a) -> Size3 { return {-a.x, -a.y, -a.z}; }
inline auto operator+(const Size3 &a, size_t b) -> Size3 { return {a.x + b, a.y + b, a.z + b}; }
inline auto operator+(size_t a, const Size3 &b) -> Size3 { return {a + b.x, a + b.y, a + b.z}; }
inline auto operator+(const Size3 &a, const Size3 &b) -> Size3 { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline auto operator-(const Size3 &a, size_t b) -> Size3 { return {a.x - b, a.y - b, a.z - b}; }
inline auto operator-(size_t a, const Size3 &b) -> Size3 { return {a - b.x, a - b.y, a - b.z}; }
inline auto operator-(const Size3 &a, const Size3 &b) -> Size3 { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
inline auto operator*(const Size3 &a, size_t b) -> Size3 { return {a.x * b, a.y * b, a.z * b}; }
inline auto operator*(size_t a, const Size3 &b) -> Size3 { return {a * b.x, a * b.y, a * b.z}; }
inline auto operator*(const Size3 &a, const Size3 &b) -> Size3 { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
inline auto operator/(const Size3 &a, size_t b) -> Size3 { return {a.x / b, a.y / b, a.z / b}; }
inline auto operator/(size_t a, const Size3 &b) -> Size3 { return {a / b.x, a / b.y, a / b.z}; }
inline auto operator/(const Size3 &a, const Size3 &b) -> Size3 { return {a.x / b.x, a.y / b.y, a.z / b.z}; }
}

using mSize3 = HinaPE::Math::Size3;
#endif //HINAPE_SIZE_H
