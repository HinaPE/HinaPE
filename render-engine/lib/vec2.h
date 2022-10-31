#ifndef HINAPE_VEC2_H
#define HINAPE_VEC2_H

#include <algorithm>
#include <cmath>
#include <ostream>

#include "log.h"

struct Vec2
{

    Vec2()
    {
        x = 0.0f;
        y = 0.0f;
    }

    explicit Vec2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    explicit Vec2(float f)
    {
        x = y = f;
    }

    explicit Vec2(int _x, int _y)
    {
        x = (float) _x;
        y = (float) _y;
    }

    Vec2(const Vec2 &) = default;
    auto operator=(const Vec2 &) -> Vec2 & = default;
    ~Vec2() = default;

    auto operator[](int idx) -> float &
    {
        assert(idx >= 0 && idx <= 1);
        return data[idx];
    }

    auto operator[](int idx) const -> float
    {
        assert(idx >= 0 && idx <= 1);
        return data[idx];
    }

    auto operator+=(Vec2 v) -> Vec2
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    auto operator-=(Vec2 v) -> Vec2
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    auto operator*=(Vec2 v) -> Vec2
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    auto operator/=(Vec2 v) -> Vec2
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    auto operator+=(float s) -> Vec2
    {
        x += s;
        y += s;
        return *this;
    }

    auto operator-=(float s) -> Vec2
    {
        x -= s;
        y -= s;
        return *this;
    }

    auto operator*=(float s) -> Vec2
    {
        x *= s;
        y *= s;
        return *this;
    }

    auto operator/=(float s) -> Vec2
    {
        x /= s;
        y /= s;
        return *this;
    }

    auto operator+(Vec2 v) const -> Vec2
    {
        return Vec2(x + v.x, y + v.y);
    }

    auto operator-(Vec2 v) const -> Vec2
    {
        return Vec2(x - v.x, y - v.y);
    }

    auto operator*(Vec2 v) const -> Vec2
    {
        return Vec2(x * v.x, y * v.y);
    }

    auto operator/(Vec2 v) const -> Vec2
    {
        return Vec2(x / v.x, y / v.y);
    }

    auto operator+(float s) const -> Vec2
    {
        return Vec2(x + s, y + s);
    }

    auto operator-(float s) const -> Vec2
    {
        return Vec2(x - s, y - s);
    }

    auto operator*(float s) const -> Vec2
    {
        return Vec2(x * s, y * s);
    }

    auto operator/(float s) const -> Vec2
    {
        return Vec2(x / s, y / s);
    }

    auto operator==(Vec2 v) const -> bool
    {
        return x == v.x && y == v.y;
    }

    auto operator!=(Vec2 v) const -> bool
    {
        return x != v.x || y != v.y;
    }

    /// Absolute value
    auto abs() const -> Vec2
    {
        return Vec2(std::abs(x), std::abs(y));
    }

    /// Negation
    auto operator-() const -> Vec2
    {
        return Vec2(-x, -y);
    }

    /// Are all members real numbers?
    auto valid() const -> bool
    {
        return std::isfinite(x) && std::isfinite(y);
    }

    /// Modify vec to have unit length
    auto normalize() -> Vec2
    {
        float n = norm();
        x /= n;
        y /= n;
        return *this;
    }

    /// Return unit length vec in the same direction
    auto unit() const -> Vec2
    {
        float n = norm();
        return Vec2(x / n, y / n);
    }

    auto norm_squared() const -> float
    {
        return x * x + y * y;
    }

    auto norm() const -> float
    {
        return std::sqrt(norm_squared());
    }

    auto range(float min, float max) const -> Vec2
    {
        if (!valid())
            return Vec2();
        Vec2 r = *this;
        float range = max - min;
        while (r.x < min)
            r.x += range;
        while (r.x >= max)
            r.x -= range;
        while (r.y < min)
            r.y += range;
        while (r.y >= max)
            r.y -= range;
        return r;
    }

    union
    {
        struct
        {
            float x;
            float y;
        };
        float data[2] = {};
    };
};

inline auto operator+(float s, Vec2 v) -> Vec2
{
    return Vec2(v.x + s, v.y + s);
}

inline auto operator-(float s, Vec2 v) -> Vec2
{
    return Vec2(v.x - s, v.y - s);
}

inline auto operator*(float s, Vec2 v) -> Vec2
{
    return Vec2(v.x * s, v.y * s);
}

inline auto operator/(float s, Vec2 v) -> Vec2
{
    return Vec2(s / v.x, s / v.y);
}

/// Take minimum of each component
inline auto hmin(Vec2 l, Vec2 r) -> Vec2
{
    return Vec2(std::min(l.x, r.x), std::min(l.y, r.y));
}

/// Take maximum of each component
inline auto hmax(Vec2 l, Vec2 r) -> Vec2
{
    return Vec2(std::max(l.x, r.x), std::max(l.y, r.y));
}

/// 2D dot product
inline auto dot(Vec2 l, Vec2 r) -> float
{
    return l.x * r.x + l.y * r.y;
}

inline auto operator<<(std::ostream &out, Vec2 v) -> std::ostream &
{
    out << "{" << v.x << "," << v.y << "}";
    return out;
}

#endif
