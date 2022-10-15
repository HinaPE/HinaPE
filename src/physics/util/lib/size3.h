#ifndef HINAPE_SIZE3_H
#define HINAPE_SIZE3_H

#include "size2.h"

namespace HinaPE
{
class Size3
{
public:
    size_t x, y, z;

public:
    template<typename U>
    auto set(const std::initializer_list<U> &lst) -> void;
    auto set(size_t s) -> void;
    auto set(size_t x, size_t y, size_t z) -> void;
    auto set(const Size2 &sz, size_t z) -> void;
    auto set(const Size3 &v) -> void;
    auto setZero() -> void;
    auto add(size_t v) const -> Size3;
    auto add(const Size3 &v) const -> Size3;
    auto sub(size_t v) const -> Size3;
    auto sub(const Size3 &v) const -> Size3;
    auto mul(size_t v) const -> Size3;
    auto mul(const Size3 &v) const -> Size3;
    auto div(size_t v) const -> Size3;
    auto div(const Size3 &v) const -> Size3;
    auto rsub(size_t v) const -> Size3;
    auto rsub(const Size3 &v) const -> Size3;
    auto rdiv(size_t v) const -> Size3;
    auto rdiv(const Size3 &v) const -> Size3;
    auto iadd(size_t v) -> void;
    auto iadd(const Size3 &v) -> void;
    auto isub(size_t v) -> void;
    auto isub(const Size3 &v) -> void;
    auto imul(size_t v) -> void;
    auto imul(const Size3 &v) -> void;
    auto idiv(size_t v) -> void;
    auto idiv(const Size3 &v) -> void;
    auto at(size_t i) const -> const size_t &;
    auto at(size_t i) -> size_t &;
    auto sum() const -> size_t;
    auto min() const -> size_t;
    auto max() const -> size_t;
    auto absmin() const -> size_t;
    auto absmax() const -> size_t;
    auto dominant_axis() const -> size_t;
    auto subminant_axis() const -> size_t;
    auto is_equal(const Size3 &other) const -> bool;

public:
    template<typename U>
    Size3(const std::initializer_list<U> &lst);
    constexpr Size3() : x(0), y(0), z(0) {}
    constexpr Size3(size_t x_, size_t y_, size_t z_) : x(x_), y(y_), z(z_) {}
    constexpr Size3(const Size2 &v, size_t z_) : x(v.x), y(v.y), z(z_) {}
    template<typename U>
    auto operator=(const std::initializer_list<U> &lst) -> Size3 &;
    auto operator[](size_t i) -> size_t &;
    auto operator[](size_t i) const -> const size_t &;
    auto operator=(const Size3 &v) -> Size3 &;
    auto operator+=(size_t v) -> Size3 &;
    auto operator+=(const Size3 &v) -> Size3 &;
    auto operator-=(size_t v) -> Size3 &;
    auto operator-=(const Size3 &v) -> Size3 &;
    auto operator*=(size_t v) -> Size3 &;
    auto operator*=(const Size3 &v) -> Size3 &;
    auto operator/=(size_t v) -> Size3 &;
    auto operator/=(const Size3 &v) -> Size3 &;
    auto operator==(const Size3 &v) const -> bool;
    auto operator!=(const Size3 &v) const -> bool;
};

template<typename U>
Size3::Size3(const std::initializer_list<U> &lst) { set(lst); }
template<typename U>
auto Size3::set(const std::initializer_list<U> &lst) -> void
{
    assert(lst.size() >= 3);

    auto input_elem = lst.begin();
    x = static_cast<U>(*input_elem);
    y = static_cast<U>(*(++input_elem));
    z = static_cast<U>(*(++input_elem));
}
auto Size3::set(size_t s) -> void { x = y = z = s; }
auto Size3::set(size_t x, size_t y, size_t z) -> void
{
    this->x = x;
    this->y = y;
    this->z = z;
}
auto Size3::set(const Size2 &sz, size_t z) -> void
{
    x = sz.x;
    y = sz.y;
    this->z = z;
}
auto Size3::set(const Size3 &v) -> void
{
    x = v.x;
    y = v.y;
    z = v.z;
}
auto Size3::setZero() -> void { x = y = z = 0; }
auto Size3::add(size_t v) const -> Size3 { return Size3(x + v, y + v, z + v); }
auto Size3::add(const Size3 &v) const -> Size3 { return Size3(x + v.x, y + v.y, z + v.z); }
auto Size3::sub(size_t v) const -> Size3 { return Size3(x - v, y - v, z - v); }
auto Size3::sub(const Size3 &v) const -> Size3 { return Size3(x - v.x, y - v.y, z - v.z); }
auto Size3::mul(size_t v) const -> Size3 { return Size3(x * v, y * v, z * v); }
auto Size3::mul(const Size3 &v) const -> Size3 { return Size3(x * v.x, y * v.y, z * v.z); }
auto Size3::div(size_t v) const -> Size3 { return Size3(x / v, y / v, z / v); }
auto Size3::div(const Size3 &v) const -> Size3 { return Size3(x / v.x, y / v.y, z / v.z); }
auto Size3::rsub(size_t v) const -> Size3 { return Size3(v - x, v - y, v - z); }
auto Size3::rsub(const Size3 &v) const -> Size3 { return Size3(v.x - x, v.y - y, v.z - z); }
auto Size3::rdiv(size_t v) const -> Size3 { return Size3(v / x, v / y, v / z); }
auto Size3::rdiv(const Size3 &v) const -> Size3 { return Size3(v.x / x, v.y / y, v.z / z); }
auto Size3::iadd(size_t v) -> void
{
    x += v;
    y += v;
    z += v;
}
auto Size3::iadd(const Size3 &v) -> void
{
    x += v.x;
    y += v.y;
    z += v.z;
}
auto Size3::isub(size_t v) -> void
{
    x -= v;
    y -= v;
    z -= v;
}
auto Size3::isub(const Size3 &v) -> void
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}
auto Size3::imul(size_t v) -> void
{
    x *= v;
    y *= v;
    z *= v;
}
auto Size3::imul(const Size3 &v) -> void
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
}
auto Size3::idiv(size_t v) -> void
{
    x /= v;
    y /= v;
    z /= v;
}
auto Size3::idiv(const Size3 &v) -> void
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
}
auto Size3::at(size_t i) const -> const size_t &
{
    assert(i < 3);
    return (&x)[i];
}
auto Size3::at(size_t i) -> size_t &
{
    assert(i < 3);
    return (&x)[i];
}
auto Size3::sum() const -> size_t { return x + y + z; }
auto Size3::min() const -> size_t { return std::min(std::min(x, y), z); }
auto Size3::max() const -> size_t { return std::max(std::max(x, y), z); }
auto Size3::absmin() const -> size_t { return std::abs(static_cast<int>(min())); }
auto Size3::absmax() const -> size_t { return std::abs(static_cast<int>(max())); }
auto Size3::dominant_axis() const -> size_t { return (x > y) ? ((x > z) ? 0 : 2) : ((y > z) ? 1 : 2); }
auto Size3::subminant_axis() const -> size_t { return (x < y) ? ((x < z) ? 0 : 2) : ((y < z) ? 1 : 2); }
auto Size3::is_equal(const Size3 &other) const -> bool { return (x == other.x && y == other.y && z == other.z); }
template<typename U>
auto Size3::operator=(const std::initializer_list<U> &lst) -> Size3 &
{
    set(lst);
    return (*this);
}
auto Size3::operator[](size_t i) -> size_t &
{
    assert(i < 3);
    return (&x)[i];
}
auto Size3::operator[](size_t i) const -> const size_t &
{
    assert(i < 3);
    return (&x)[i];
}
auto Size3::operator=(const Size3 &v) -> Size3 &
{
    set(v);
    return (*this);
}
auto Size3::operator+=(size_t v) -> Size3 &
{
    iadd(v);
    return (*this);
}
auto Size3::operator+=(const Size3 &v) -> Size3 &
{
    iadd(v);
    return (*this);
}
auto Size3::operator-=(size_t v) -> Size3 &
{
    isub(v);
    return (*this);
}
auto Size3::operator-=(const Size3 &v) -> Size3 &
{
    isub(v);
    return (*this);
}
auto Size3::operator*=(size_t v) -> Size3 &
{
    imul(v);
    return (*this);
}
auto Size3::operator*=(const Size3 &v) -> Size3 &
{
    imul(v);
    return (*this);
}
auto Size3::operator/=(size_t v) -> Size3 &
{
    idiv(v);
    return (*this);
}
auto Size3::operator/=(const Size3 &v) -> Size3 &
{
    idiv(v);
    return (*this);
}
auto Size3::operator==(const Size3 &v) const -> bool { return is_equal(v); }
auto Size3::operator!=(const Size3 &v) const -> bool { return !is_equal(v); }

inline auto operator+(const Size3 &a) -> Size3 { return a; }
inline auto operator+(const Size3 &a, size_t b) -> Size3 { return a.add(b); }
inline auto operator+(size_t a, const Size3 &b) -> Size3 { return b.add(a); }
inline auto operator+(const Size3 &a, const Size3 &b) -> Size3 { return a.add(b); }
inline auto operator-(const Size3 &a, size_t b) -> Size3 { return a.sub(b); }
inline auto operator-(size_t a, const Size3 &b) -> Size3 { return b.rsub(a); }
inline auto operator-(const Size3 &a, const Size3 &b) -> Size3 { return a.sub(b); }
inline auto operator*(const Size3 &a, size_t b) -> Size3 { return a.mul(b); }
inline auto operator*(size_t a, const Size3 &b) -> Size3 { return b.mul(a); }
inline auto operator*(const Size3 &a, const Size3 &b) -> Size3 { return a.mul(b); }
inline auto operator/(const Size3 &a, size_t b) -> Size3 { return a.div(b); }
inline auto operator/(size_t a, const Size3 &b) -> Size3 { return b.rdiv(a); }
inline auto operator/(const Size3 &a, const Size3 &b) -> Size3 { return a.div(b); }
inline auto min(const Size3 &a, const Size3 &b) -> Size3 { return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)}; }
inline auto max(const Size3 &a, const Size3 &b) -> Size3 { return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)}; }
inline auto clamp(const Size3 &v, const Size3 &low, const Size3 &high) -> Size3
{
    return {std::clamp(v.x, low.x, high.x), std::clamp(v.y, low.y, high.y), std::clamp(v.z, low.z, high.z)};
}
}

#endif //HINAPE_SIZE3_H
