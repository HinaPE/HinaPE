#ifndef HINAPE_POINT3_H
#define HINAPE_POINT3_H

#include "point2.h"
#include <algorithm>

// TODO:

namespace HinaPE
{
template<typename T>
class Point<T, 3>
{
public:
    T x, y, z;

public:

public:
    template <typename U>
    Point(const std::initializer_list<U>& lst);
    constexpr Point() : x(0), y(0), z(0) {}
    constexpr Point(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
    constexpr Point(const Point2<T>& v, T z_) : x(v.x), y(v.y), z(z_) {}
    constexpr Point(const Point& v) : x(v.x), y(v.y), z(v.z) {}

public:
    static_assert(std::is_arithmetic<T>::value, "Point only can be instantiated with arithmetic types");
};
}

#endif //HINAPE_POINT3_H
