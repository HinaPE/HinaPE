#ifndef HINAPE_SPECTRUM_H
#define HINAPE_SPECTRUM_H

#include "vec3.h"
#include <cmath>
#include <ostream>

struct Spectrum
{

    Spectrum()
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
    }

    explicit Spectrum(float _r, float _g, float _b)
    {
        r = _r;
        g = _g;
        b = _b;
    }

    explicit Spectrum(float f)
    {
        r = g = b = f;
    }

    Spectrum(const Spectrum &) = default;
    auto operator=(const Spectrum &) -> Spectrum & = default;
    ~Spectrum() = default;

    auto operator+=(Spectrum v) -> Spectrum
    {
        r += v.r;
        g += v.g;
        b += v.b;
        return *this;
    }

    auto operator*=(Spectrum v) -> Spectrum
    {
        r *= v.r;
        g *= v.g;
        b *= v.b;
        return *this;
    }

    auto operator*=(float s) -> Spectrum
    {
        r *= s;
        g *= s;
        b *= s;
        return *this;
    }

    static auto direction(Vec3 v) -> Spectrum
    {
        v.normalize();
        Spectrum s(std::abs(v.x), std::abs(v.y), std::abs(v.z));
        s.to_linear();
        return s;
    }

    static auto to_linear(float f) -> float
    {
        if (f > 0.04045f)
        {
            return std::pow((f + 0.055f) / 1.055f, 2.4f);
        } else
        {
            return f / 12.92f;
        }
    }

    static auto to_srgb(float f) -> float
    {
        if (f > 0.0031308f)
        {
            return 1.055f * (std::pow(f, (1.0f / 2.4f))) - 0.055f;
        } else
        {
            return f * 12.92f;
        }
    }

    auto to_srgb() const -> Spectrum
    {
        Spectrum ret;
        ret.r = to_srgb(r);
        ret.g = to_srgb(g);
        ret.b = to_srgb(b);
        return ret;
    }

    auto to_linear() const -> Spectrum
    {
        Spectrum ret;
        ret.r = to_linear(r);
        ret.g = to_linear(g);
        ret.b = to_linear(b);
        return ret;
    }

    auto operator+(Spectrum v) const -> Spectrum
    {
        return Spectrum(r + v.r, g + v.g, b + v.b);
    }

    auto operator-(Spectrum v) const -> Spectrum
    {
        return Spectrum(r - v.r, g - v.g, b - v.b);
    }

    auto operator*(Spectrum v) const -> Spectrum
    {
        return Spectrum(r * v.r, g * v.g, b * v.b);
    }

    auto operator+(float s) const -> Spectrum
    {
        return Spectrum(r + s, g + s, b + s);
    }

    auto operator*(float s) const -> Spectrum
    {
        return Spectrum(r * s, g * s, b * s);
    }

    auto operator/(float s) const -> Spectrum
    {
        return Spectrum(r / s, g / s, b / s);
    }

    auto operator==(Spectrum v) const -> bool
    {
        return r == v.r && g == v.g && b == v.b;
    }

    auto operator!=(Spectrum v) const -> bool
    {
        return r != v.r || g != v.g || b != v.b;
    }

    auto luma() const -> float
    {
        return 0.2126f * r + 0.7152f * g + 0.0722f * b;
    }

    auto valid() const -> bool
    {
        return std::isfinite(r) && std::isfinite(g) && std::isfinite(b);
    }

    auto to_vec() const -> Vec3
    {
        return Vec3(r, g, b);
    }

    union
    {
        struct
        {
            float r;
            float g;
            float b;
        };
        float data[3] = {};
    };
};

inline auto operator+(float s, Spectrum v) -> Spectrum
{
    return Spectrum(v.r + s, v.g + s, v.b + s);
}

inline auto operator*(float s, Spectrum v) -> Spectrum
{
    return Spectrum(v.r * s, v.g * s, v.b * s);
}

inline auto operator<<(std::ostream &out, Spectrum v) -> std::ostream &
{
    out << "Spectrum{" << v.r << "," << v.g << "," << v.b << "}";
    return out;
}

#endif
