#ifndef HINAPE_VEC3_H
#define HINAPE_VEC3_H

#include <algorithm>
#include <cmath>
#include <ostream>

#include "log.h"

struct Vec3
{

    Vec3()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    explicit Vec3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    explicit Vec3(int _x, int _y, int _z)
    {
        x = (float) _x;
        y = (float) _y;
        z = (float) _z;
    }

    explicit Vec3(float f)
    {
        x = y = z = f;
    }

    Vec3(const Vec3 &) = default;
    auto operator=(const Vec3 &) -> Vec3 & = default;
    ~Vec3() = default;

    auto operator[](int idx) -> float &
    {
        assert(idx >= 0 && idx <= 2);
        return data[idx];
    }

    auto operator[](int idx) const -> float
    {
        assert(idx >= 0 && idx <= 2);
        return data[idx];
    }

    auto operator+=(Vec3 v) -> Vec3
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    auto operator-=(Vec3 v) -> Vec3
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    auto operator*=(Vec3 v) -> Vec3
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    auto operator/=(Vec3 v) -> Vec3
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    auto operator+=(float s) -> Vec3
    {
        x += s;
        y += s;
        z += s;
        return *this;
    }

    auto operator-=(float s) -> Vec3
    {
        x -= s;
        y -= s;
        z -= s;
        return *this;
    }

    auto operator*=(float s) -> Vec3
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    auto operator/=(float s) -> Vec3
    {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    auto operator+(Vec3 v) const -> Vec3
    {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    auto operator-(Vec3 v) const -> Vec3
    {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    auto operator*(Vec3 v) const -> Vec3
    {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

    auto operator/(Vec3 v) const -> Vec3
    {
        return Vec3(x / v.x, y / v.y, z / v.z);
    }

    auto operator+(float s) const -> Vec3
    {
        return Vec3(x + s, y + s, z + s);
    }

    auto operator-(float s) const -> Vec3
    {
        return Vec3(x - s, y - s, z - s);
    }

    auto operator*(float s) const -> Vec3
    {
        return Vec3(x * s, y * s, z * s);
    }

    auto operator/(float s) const -> Vec3
    {
        return Vec3(x / s, y / s, z / s);
    }

    auto operator==(Vec3 v) const -> bool
    {
        return x == v.x && y == v.y && z == v.z;
    }

    auto operator!=(Vec3 v) const -> bool
    {
        return x != v.x || y != v.y || z != v.z;
    }

    /// Absolute value
    auto abs() const -> Vec3
    {
        return Vec3(std::abs(x), std::abs(y), std::abs(z));
    }

    /// Negation
    auto operator-() const -> Vec3
    {
        return Vec3(-x, -y, -z);
    }

    /// Are all members real numbers?
    auto valid() const -> bool
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }

    /// Modify vec to have unit length
    auto normalize() -> Vec3
    {
        float n = norm();
        x /= n;
        y /= n;
        z /= n;
        return *this;
    }

    /// Return unit length vec in the same direction
    auto unit() const -> Vec3
    {
        float n = norm();
        return Vec3(x / n, y / n, z / n);
    }

    auto norm_squared() const -> float
    {
        return x * x + y * y + z * z;
    }

    auto norm() const -> float
    {
        return std::sqrt(norm_squared());
    }

    /// Make sure all components are in the range [min,max) with floating point mod logic
    auto range(float min, float max) const -> Vec3
    {
        if (!valid())
            return Vec3();
        Vec3 r = *this;
        float range = max - min;
        while (r.x < min)
            r.x += range;
        while (r.x >= max)
            r.x -= range;
        while (r.y < min)
            r.y += range;
        while (r.y >= max)
            r.y -= range;
        while (r.z < min)
            r.z += range;
        while (r.z >= max)
            r.z -= range;
        return r;
    }

    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float data[3] = {};
    };
};

inline auto operator+(float s, Vec3 v) -> Vec3
{
    return Vec3(v.x + s, v.y + s, v.z + s);
}

inline auto operator-(float s, Vec3 v) -> Vec3
{
    return Vec3(v.x - s, v.y - s, v.z - s);
}

inline auto operator*(float s, Vec3 v) -> Vec3
{
    return Vec3(v.x * s, v.y * s, v.z * s);
}

inline auto operator/(float s, Vec3 v) -> Vec3
{
    return Vec3(s / v.x, s / v.y, s / v.z);
}

/// Take minimum of each component
inline auto hmin(Vec3 l, Vec3 r) -> Vec3
{
    return Vec3(std::min(l.x, r.x), std::min(l.y, r.y), std::min(l.z, r.z));
}

/// Take maximum of each component
inline auto hmax(Vec3 l, Vec3 r) -> Vec3
{
    return Vec3(std::max(l.x, r.x), std::max(l.y, r.y), std::max(l.z, r.z));
}

/// 3D dot product
inline auto dot(Vec3 l, Vec3 r) -> float
{
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

/// 3D cross product
inline auto cross(Vec3 l, Vec3 r) -> Vec3
{
    return Vec3(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
}

inline auto operator<<(std::ostream &out, Vec3 v) -> std::ostream &
{
    out << "{" << v.x << "," << v.y << "," << v.z << "}";
    return out;
}

inline auto operator<(Vec3 l, Vec3 r) -> bool
{
    if (l.x == r.x)
    {
        if (l.y == r.y)
        {
            return l.z < r.z;
        }
        return l.y < r.y;
    }
    return l.x < r.x;
}

#endif
