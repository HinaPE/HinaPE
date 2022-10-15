#ifndef HINAPE_SIZE2_H
#define HINAPE_SIZE2_H

#include "size.h"

namespace HinaPE
{
class Size2
{
public:
    size_t x, y;

public:
    template<typename U>
    auto set(const std::initializer_list<U> &lst) -> void;
    auto set(size_t s) -> void;
    auto set(size_t x, size_t y) -> void;
    auto set(const Size2 &v) -> void;
    auto setZero() -> void;
    auto add(size_t v) const -> Size2;
    auto add(const Size2 &v) const -> Size2;
    auto sub(size_t v) const -> Size2;
    auto sub(const Size2 &v) const -> Size2;
    auto mul(size_t v) const -> Size2;
    auto mul(const Size2 &v) const -> Size2;
    auto div(size_t v) const -> Size2;
    auto div(const Size2 &v) const -> Size2;
    auto rsub(size_t v) const -> Size2;
    auto rsub(const Size2 &v) const -> Size2;
    auto rdiv(size_t v) const -> Size2;
    auto rdiv(const Size2 &v) const -> Size2;
    auto iadd(size_t v) -> void;
    auto iadd(const Size2 &v) -> void;
    auto isub(size_t v) -> void;
    auto isub(const Size2 &v) -> void;
    auto imul(size_t v) -> void;
    auto imul(const Size2 &v) -> void;
    auto idiv(size_t v) -> void;
    auto idiv(const Size2 &v) -> void;
    auto at(size_t i) const -> const size_t &;
    auto at(size_t i) -> size_t &;
    auto sum() const -> size_t;
    auto min() const -> size_t;
    auto max() const -> size_t;
    auto absmin() const -> size_t;
    auto absmax() const -> size_t;
    auto dominant_axis() const -> size_t;
    auto subminant_axis() const -> size_t;
    auto is_equal(const Size2 &other) const -> bool;

public:
    template<typename U>
    Size2(const std::initializer_list<U> &lst);
    constexpr Size2() : x(0), y(0) {}
    constexpr Size2(size_t x_, size_t y_) : x(x_), y(y_) {}
    constexpr Size2(const Size2 &v) = default;
    template<typename U>
    auto operator=(const std::initializer_list<U> &lst) -> Size2 &;
    auto operator[](size_t i) -> size_t &;
    auto operator[](size_t i) const -> const size_t &;
    auto operator=(const Size2 &v) -> Size2 &;
    auto operator+=(size_t v) -> Size2 &;
    auto operator+=(const Size2 &v) -> Size2 &;
    auto operator-=(size_t v) -> Size2 &;
    auto operator-=(const Size2 &v) -> Size2 &;
    auto operator*=(size_t v) -> Size2 &;
    auto operator*=(const Size2 &v) -> Size2 &;
    auto operator/=(size_t v) -> Size2 &;
    auto operator/=(const Size2 &v) -> Size2 &;
    auto operator==(const Size2 &v) const -> bool;
    auto operator!=(const Size2 &v) const -> bool;
};

template<typename U>
Size2::Size2(const std::initializer_list<U> &lst) { set(lst); }
template<typename U>
auto Size2::set(const std::initializer_list<U> &lst) -> void
{
    assert(lst.size() >= 2);

    auto input_elem = lst.begin();
    x = static_cast<size_t>(*input_elem);
    y = static_cast<size_t>(*(++input_elem));
}
auto Size2::set(size_t s) -> void { x = y = s; }
auto Size2::set(size_t x, size_t y) -> void
{
    this->x = x;
    this->y = y;
}
auto Size2::set(const Size2 &v) -> void
{
    x = v.x;
    y = v.y;
}
auto Size2::setZero() -> void { x = y = 0; }
auto Size2::add(size_t v) const -> Size2 { return {x + v, y + v}; }
auto Size2::add(const Size2 &v) const -> Size2 { return {x + v.x, y + v.y}; }
auto Size2::sub(size_t v) const -> Size2 { return {x - v, y - v}; }
auto Size2::sub(const Size2 &v) const -> Size2 { return {x - v.x, y - v.y}; }
auto Size2::mul(size_t v) const -> Size2 { return {x * v, y * v}; }
auto Size2::mul(const Size2 &v) const -> Size2 { return {x * v.x, y * v.y}; }
auto Size2::div(size_t v) const -> Size2 { return {x / v, y / v}; }
auto Size2::div(const Size2 &v) const -> Size2 { return {x / v.x, y / v.y}; }
auto Size2::rsub(size_t v) const -> Size2 { return {v - x, v - y}; }
auto Size2::rsub(const Size2 &v) const -> Size2 { return {v.x - x, v.y - y}; }
auto Size2::rdiv(size_t v) const -> Size2 { return {v / x, v / y}; }
auto Size2::rdiv(const Size2 &v) const -> Size2 { return {v.x / x, v.y / y}; }
auto Size2::iadd(size_t v) -> void
{
    x += v;
    y += v;
}
auto Size2::iadd(const Size2 &v) -> void
{
    x += v.x;
    y += v.y;
}
auto Size2::isub(size_t v) -> void
{
    x -= v;
    y -= v;
}
auto Size2::isub(const Size2 &v) -> void
{
    x -= v.x;
    y -= v.y;
}
auto Size2::imul(size_t v) -> void
{
    x *= v;
    y *= v;
}
auto Size2::imul(const Size2 &v) -> void
{
    x *= v.x;
    y *= v.y;
}
auto Size2::idiv(size_t v) -> void
{
    x /= v;
    y /= v;
}
auto Size2::idiv(const Size2 &v) -> void
{
    x /= v.x;
    y /= v.y;
}
auto Size2::at(size_t i) const -> const size_t &
{
    assert(i < 2);
    return (&x)[i];
}
auto Size2::at(size_t i) -> size_t &
{
    assert(i < 2);
    return (&x)[i];
}
auto Size2::sum() const -> size_t { return x + y; }
auto Size2::min() const -> size_t { return std::min(x, y); }
auto Size2::max() const -> size_t { return std::max(x, y); }
auto Size2::absmin() const -> size_t { return std::abs(static_cast<int>(std::min(x, y))); }
auto Size2::absmax() const -> size_t { return std::abs(static_cast<int>(std::max(x, y))); }
auto Size2::dominant_axis() const -> size_t { return (x > y) ? 0 : 1; }
auto Size2::subminant_axis() const -> size_t { return (x < y) ? 0 : 1; }
auto Size2::is_equal(const Size2 &other) const -> bool { return (x == other.x && y == other.y); }
template<typename U>
auto Size2::operator=(const std::initializer_list<U> &lst) -> Size2 &
{
    set(lst);
    return (*this);
}
auto Size2::operator[](size_t i) -> size_t &
{
    assert(i < 2);
    return (&x)[i];
}
auto Size2::operator[](size_t i) const -> const size_t &
{
    assert(i < 2);
    return (&x)[i];
}
auto Size2::operator=(const Size2 &v) -> Size2 &
{
    set(v);
    return (*this);
}
auto Size2::operator+=(size_t v) -> Size2 &
{
    iadd(v);
    return (*this);
}
auto Size2::operator+=(const Size2 &v) -> Size2 &
{
    iadd(v);
    return (*this);
}
auto Size2::operator-=(size_t v) -> Size2 &
{
    isub(v);
    return (*this);
}
auto Size2::operator-=(const Size2 &v) -> Size2 &
{
    isub(v);
    return (*this);
}
auto Size2::operator*=(size_t v) -> Size2 &
{
    imul(v);
    return (*this);
}
auto Size2::operator*=(const Size2 &v) -> Size2 &
{
    imul(v);
    return (*this);
}
auto Size2::operator/=(size_t v) -> Size2 &
{
    idiv(v);
    return (*this);
}
auto Size2::operator/=(const Size2 &v) -> Size2 &
{
    idiv(v);
    return (*this);
}
auto Size2::operator==(const Size2 &v) const -> bool { return is_equal(v); }
auto Size2::operator!=(const Size2 &v) const -> bool { return !is_equal(v); }
inline auto operator+(const Size2 &a) -> Size2 { return a; }
inline auto operator+(const Size2 &a, size_t b) -> Size2 { return a.add(b); }
inline auto operator+(size_t a, const Size2 &b) -> Size2 { return b.add(a); }
inline auto operator+(const Size2 &a, const Size2 &b) -> Size2 { return a.add(b); }
inline auto operator-(const Size2 &a, size_t b) -> Size2 { return a.sub(b); }
inline auto operator-(size_t a, const Size2 &b) -> Size2 { return b.rsub(a); }
inline auto operator-(const Size2 &a, const Size2 &b) -> Size2 { return a.sub(b); }
inline auto operator*(const Size2 &a, size_t b) -> Size2 { return a.mul(b); }
inline auto operator*(size_t a, const Size2 &b) -> Size2 { return b.mul(a); }
inline auto operator*(const Size2 &a, const Size2 &b) -> Size2 { return a.mul(b); }
inline auto operator/(const Size2 &a, size_t b) -> Size2 { return a.div(b); }
inline auto operator/(size_t a, const Size2 &b) -> Size2 { return b.rdiv(a); }
inline auto operator/(const Size2 &a, const Size2 &b) -> Size2 { return a.div(b); }
inline auto min(const Size2 &a, const Size2 &b) -> Size2 { return {std::min(a.x, b.x), std::min(a.y, b.y)}; }
inline auto max(const Size2 &a, const Size2 &b) -> Size2 { return {std::max(a.x, b.x), std::max(a.y, b.y)}; }
inline auto clamp(const Size2 &v, const Size2 &low, const Size2 &high) -> Size2 { return {std::clamp(v.x, low.x, high.x), std::clamp(v.y, low.y, high.y)}; }
}

#endif //HINAPE_SIZE2_H
