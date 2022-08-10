#ifndef HINAPE_POINT2_H
#define HINAPE_POINT2_H

#include "point.h"

#include <array>
#include <type_traits>

namespace HinaPE
{
template<typename T>
class Point<T, 2> final
{
public:
    T x, y;

public:
    template<typename U>
    auto set(const std::initializer_list<U> &lst) -> void;
    auto set(T s) -> void;
    auto set(T x, T y) -> void;
    auto set(const Point &pt) -> void;
    auto setZero() -> void;
    auto add(T v) const -> Point;
    auto add(const Point &v) const -> Point;
    auto sub(T v) const -> Point;
    auto sub(const Point &v) const -> Point;
    auto mul(T v) const -> Point;
    auto mul(const Point &v) const -> Point;
    auto div(T v) const -> Point;
    auto div(const Point &v) const -> Point;
    auto rsub(T v) const -> Point;
    auto rsub(const Point &v) const -> Point;
    auto rdiv(T v) const -> Point;
    auto rdiv(const Point &v) const -> Point;

public:
    template<typename U>
    Point(const std::initializer_list<U> &lst);
    constexpr Point() : x(0), y(0) {}
    constexpr Point(T x_, T y_) : x(x_), y(y_) {}
    constexpr Point(const Point &v) : x(v.x), y(v.y) {}
    constexpr Point(Point &&v)  noexcept : x(v.x), y(v.y) {}
    constexpr ~Point() = default;
    constexpr auto operator=(const Point &v) -> Point & { x = v.x; y = v.y; return *this; }
    constexpr auto operator=(Point &&v) noexcept -> Point & { x = v.x; y = v.y; return *this; }
    constexpr auto operator==(const Point &v) const -> bool { return x == v.x && y == v.y; }
    constexpr auto operator!=(const Point &v) const -> bool { return x != v.x || y != v.y; }
    constexpr auto operator+(const Point &v) const -> Point { return add(v); }
    constexpr auto operator-(const Point &v) const -> Point { return sub(v); }
    constexpr auto operator*(const Point &v) const -> Point { return mul(v); }
    constexpr auto operator/(const Point &v) const -> Point { return div(v); }
    constexpr auto operator-() const -> Point { return Point(-x, -y); }
    constexpr auto operator+=(const Point &v) -> Point & { return set(add(v)); }
    constexpr auto operator-=(const Point &v) -> Point & { return set(sub(v)); }
    constexpr auto operator*=(const Point &v) -> Point & { return set(mul(v)); }
    constexpr auto operator/=(const Point &v) -> Point & { return set(div(v)); }
    constexpr auto operator+=(T v) -> Point & { return set(add(v)); }
    constexpr auto operator-=(T v) -> Point & { return set(sub(v)); }
    constexpr auto operator*=(T v) -> Point & { return set(mul(v)); }
    constexpr auto operator/=(T v) -> Point & { return set(div(v)); }
    constexpr auto operator[](int i) const -> T { return i == 0 ? x : y; }
    constexpr auto operator[](int i) -> T & { return i == 0 ? x : y; }
    constexpr auto operator()(int i) const -> T { return i == 0 ? x : y; }
    constexpr auto operator()(int i) -> T & { return i == 0 ? x : y; }

public:
    static_assert(std::is_arithmetic<T>::value, "Point only can be instantiated with arithmetic types");
};

template <typename T>
using Point2 = Point<T, 2>;

template<typename T>
template<typename U>
Point<T, 2>::Point(const std::initializer_list<U> &lst) { set(lst); }
template<typename T>
template<typename U>
void Point<T, 2>::set(const std::initializer_list<U> &lst)
{
    assert(lst.size() >= 2);

    auto input_elem = lst.begin();
    x = static_cast<T>(*input_elem);
    y = static_cast<T>(*(++input_elem));
}
template<typename T>
auto Point<T, 2>::set(T s) -> void
{
    x = s;
    y = s;
}
template<typename T>
auto Point<T, 2>::set(T newX, T newY) -> void
{
    x = newX;
    y = newY;
}
template<typename T>
auto Point<T, 2>::set(const Point &pt) -> void
{
    x = pt.x;
    y = pt.y;
}
template<typename T>
auto Point<T, 2>::setZero() -> void { x = y = 0; }
template<typename T>
auto Point<T, 2>::add(T v) const -> Point { return Point(x + v, y + v); }
template<typename T>
auto Point<T, 2>::add(const Point &v) const -> Point { return Point(x + v.x, y + v.y); }
template<typename T>
auto Point<T, 2>::sub(T v) const -> Point { return Point(x - v, y - v); }
template<typename T>
auto Point<T, 2>::sub(const Point &v) const -> Point { return Point(x - v.x, y - v.y); }
template<typename T>
auto Point<T, 2>::mul(T v) const -> Point { return Point(x * v, y * v); }
template<typename T>
auto Point<T, 2>::mul(const Point &v) const -> Point { return Point(x * v.x, y * v.y); }
template<typename T>
auto Point<T, 2>::div(T v) const -> Point { return Point(x / v, y / v); }
template<typename T>
auto Point<T, 2>::div(const Point &v) const -> Point { return Point(x / v.x, y / v.y); }
template<typename T>
auto Point<T, 2>::rsub(T v) const -> Point { return Point(v - x, v - y); }
template<typename T>
auto Point<T, 2>::rsub(const Point &v) const -> Point { return Point(v.x - x, v.y - y); }
template<typename T>
auto Point<T, 2>::rdiv(T v) const -> Point { return Point(v / x, v / y); }
template<typename T>
auto Point<T, 2>::rdiv(const Point &v) const -> Point { return Point(v.x / x, v.y / y); }
}

#endif //HINAPE_POINT2_H
