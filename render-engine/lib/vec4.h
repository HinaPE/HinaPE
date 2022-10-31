#ifndef HINAPE_VEC4_H
#define HINAPE_VEC4_H

#include <algorithm>
#include <cmath>
#include <ostream>

#include "log.h"
#include "vec3.h"

struct Vec4
{

    Vec4()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    explicit Vec4(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    explicit Vec4(float f)
    {
        x = y = z = w = f;
    }

    explicit Vec4(int _x, int _y, int _z, int _w)
    {
        x = (float) _x;
        y = (float) _y;
        z = (float) _z;
        w = (float) _w;
    }

    explicit Vec4(Vec3 xyz, float _w)
    {
        x = xyz.x;
        y = xyz.y;
        z = xyz.z;
        w = _w;
    }

    Vec4(const Vec4 &) = default;
    auto operator=(const Vec4 &) -> Vec4 & = default;
    ~Vec4() = default;

    auto operator[](int idx) -> float &
    {
        assert(idx >= 0 && idx <= 3);
        return data[idx];
    }

    auto operator[](int idx) const -> float
    {
        assert(idx >= 0 && idx <= 3);
        return data[idx];
    }

    auto operator+=(Vec4 v) -> Vec4
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    auto operator-=(Vec4 v) -> Vec4
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    auto operator*=(Vec4 v) -> Vec4
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }

    auto operator/=(Vec4 v) -> Vec4
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
        return *this;
    }

    auto operator+=(float s) -> Vec4
    {
        x += s;
        y += s;
        z += s;
        w += s;
        return *this;
    }

    auto operator-=(float s) -> Vec4
    {
        x -= s;
        y -= s;
        z -= s;
        w -= s;
        return *this;
    }

    auto operator*=(float s) -> Vec4
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    auto operator/=(float s) -> Vec4
    {
        x /= s;
        y /= s;
        z /= s;
        w /= s;
        return *this;
    }

    auto operator+(Vec4 v) const -> Vec4
    {
        return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    auto operator-(Vec4 v) const -> Vec4
    {
        return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    auto operator*(Vec4 v) const -> Vec4
    {
        return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
    }

    auto operator/(Vec4 v) const -> Vec4
    {
        return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);
    }

    auto operator+(float s) const -> Vec4
    {
        return Vec4(x + s, y + s, z + s, w + s);
    }

    auto operator-(float s) const -> Vec4
    {
        return Vec4(x - s, y - s, z - s, w - s);
    }

    auto operator*(float s) const -> Vec4
    {
        return Vec4(x * s, y * s, z * s, w * s);
    }

    auto operator/(float s) const -> Vec4
    {
        return Vec4(x / s, y / s, z / s, w / s);
    }

    auto operator==(Vec4 v) const -> bool
    {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    auto operator!=(Vec4 v) const -> bool
    {
        return x != v.x || y != v.y || z != v.z || w != v.w;
    }

    /// Absolute value
    auto abs() const -> Vec4
    {
        return Vec4(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
    }

    /// Negation
    auto operator-() const -> Vec4
    {
        return Vec4(-x, -y, -z, -w);
    }

    /// Are all members real numbers?
    auto valid() const -> bool
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(w);
    }

    /// Modify vec to have unit length
    auto normalize() -> Vec4
    {
        float n = norm();
        x /= n;
        y /= n;
        z /= n;
        w /= n;
        return *this;
    }

    /// Return unit length vec in the same direction
    auto unit() const -> Vec4
    {
        float n = norm();
        return Vec4(x / n, y / n, z / n, w / n);
    }

    auto norm_squared() const -> float
    {
        return x * x + y * y + z * z + w * w;
    }

    auto norm() const -> float
    {
        return std::sqrt(norm_squared());
    }

    /// Returns first three components
    auto xyz() const -> Vec3
    {
        return Vec3(x, y, z);
    }

    /// Performs perspective division (xyz/w)
    auto project() const -> Vec3
    {
        return Vec3(x / w, y / w, z / w);
    }

    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float data[4] = {};
    };
};

inline auto operator+(float s, Vec4 v) -> Vec4
{
    return Vec4(v.x + s, v.y + s, v.z + s, v.w + s);
}

inline auto operator-(float s, Vec4 v) -> Vec4
{
    return Vec4(v.x - s, v.y - s, v.z - s, v.w - s);
}

inline auto operator*(float s, Vec4 v) -> Vec4
{
    return Vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

inline auto operator/(float s, Vec4 v) -> Vec4
{
    return Vec4(s / v.x, s / v.y, s / v.z, s / v.w);
}

/// Take minimum of each component
inline auto hmin(Vec4 l, Vec4 r) -> Vec4
{
    return Vec4(std::min(l.x, r.x), std::min(l.y, r.y), std::min(l.z, r.z), std::min(l.w, r.w));
}

/// Take maximum of each component
inline auto hmax(Vec4 l, Vec4 r) -> Vec4
{
    return Vec4(std::max(l.x, r.x), std::max(l.y, r.y), std::max(l.z, r.z), std::max(l.w, r.w));
}

/// 4D dot product
inline auto dot(Vec4 l, Vec4 r) -> float
{
    return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
}

inline auto operator<<(std::ostream &out, Vec4 v) -> std::ostream &
{
    out << "{" << v.x << "," << v.y << "," << v.z << "," << v.w << "}";
    return out;
}

#endif
