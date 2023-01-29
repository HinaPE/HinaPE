#ifndef HINAPE_SIZE_H
#define HINAPE_SIZE_H

#include <cstddef>
#include <array>
#include <type_traits>

namespace Hina::Base
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
}

#endif //HINAPE_SIZE_H
