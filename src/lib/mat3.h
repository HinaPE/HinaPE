#ifndef HINAPE_MAT3_H
#define HINAPE_MAT3_H

#include "vec3.h"

struct Mat3
{

    /// Identity matrix
    static const Mat3 I;
    /// Zero matrix
    static const Mat3 Zero;

    static Mat3 transpose(const Mat3 &m);

    static Mat3 inverse(const Mat3 &m);

    Mat3()
    { *this = I; }

    explicit Mat3(Vec3 x, Vec3 y, Vec3 z)
    {
        cols[0] = x;
        cols[1] = y;
        cols[2] = z;
    }

    Mat3(const Mat3 &) = default;
    Mat3 &operator=(const Mat3 &) = default;
    ~Mat3() = default;

    Vec3 &operator[](int idx)
    {
        assert(idx >= 0 && idx <= 2);
        return cols[idx];
    }

    Vec3 operator[](int idx) const
    {
        assert(idx >= 0 && idx <= 2);
        return cols[idx];
    }

    Mat3 operator+=(const Mat3 &m)
    {
        for (int i = 0; i < 3; i++) cols[i] += m.cols[i];
        return *this;
    }

    Mat3 operator-=(const Mat3 &m)
    {
        for (int i = 0; i < 3; i++) cols[i] -= m.cols[i];
        return *this;
    }

    Mat3 operator+=(float s)
    {
        for (auto &col: cols) col += s;
        return *this;
    }

    Mat3 operator-=(float s)
    {
        for (auto &col: cols) col -= s;
        return *this;
    }

    Mat3 operator*=(float s)
    {
        for (auto &col: cols) col *= s;
        return *this;
    }

    Mat3 operator/=(float s)
    {
        for (auto &col: cols) col /= s;
        return *this;
    }

    Mat3 operator+(const Mat3 &m) const
    {
        Mat3 r;
        for (int i = 0; i < 3; i++) r.cols[i] = cols[i] + m.cols[i];
        return r;
    }

    Mat3 operator-(const Mat3 &m) const
    {
        Mat3 r;
        for (int i = 0; i < 3; i++) r.cols[i] = cols[i] - m.cols[i];
        return r;
    }

    Mat3 operator+(float s) const
    {
        Mat3 r;
        for (int i = 0; i < 3; i++) r.cols[i] = cols[i] + s;
        return r;
    }

    Mat3 operator-(float s) const
    {
        Mat3 r;
        for (int i = 0; i < 3; i++) r.cols[i] = cols[i] - s;
        return r;
    }

    Mat3 operator*(float s) const
    {
        Mat3 r;
        for (int i = 0; i < 3; i++) r.cols[i] = cols[i] * s;
        return r;
    }

    Mat3 operator/(float s) const
    {
        Mat3 r;
        for (int i = 0; i < 3; i++) r.cols[i] = cols[i] / s;
        return r;
    }

    Mat3 operator*=(const Mat3 &m)
    {
        *this = *this * m;
        return *this;
    }

    Mat3 operator*(const Mat3 &m) const
    {
        Mat3 ret;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                ret[i][j] = 0.0f;
                for (int k = 0; k < 4; k++)
                {
                    ret[i][j] += m[i][k] * cols[k][j];
                }
            }
        }
        return ret;
    }

    Vec3 operator*(Vec3 v) const
    {
        return v[0] * cols[0] + v[1] * cols[1] + v[2] * cols[2];
    }

    Mat3 T() const
    {
        return transpose(*this);
    }

    Mat3 inverse() const
    {
        return inverse(*this);
    }

    float det() const
    {
        // TODO:
        assert(false);
    }

    union
    {
        Vec3 cols[3];
        float data[9] = {};
    };
};

inline bool operator==(const Mat3 &l, const Mat3 &r)
{
    for (int i = 0; i < 9; i++)
        if (l.data[i] != r.data[i]) return false;
    return true;
}

inline bool operator!=(const Mat3 &l, const Mat3 &r)
{
    for (int i = 0; i < 9; i++)
        if (l.data[i] != r.data[i]) return true;
    return false;
}

inline Mat3 operator+(float s, const Mat3 &m)
{
    Mat3 r;
    for (int i = 0; i < 3; i++) r.cols[i] = m.cols[i] + s;
    return r;
}

inline Mat3 operator-(float s, const Mat3 &m)
{
    Mat3 r;
    for (int i = 0; i < 3; i++) r.cols[i] = m.cols[i] - s;
    return r;
}

inline Mat3 operator*(float s, const Mat3 &m)
{
    Mat3 r;
    for (int i = 0; i < 3; i++) r.cols[i] = m.cols[i] * s;
    return r;
}

inline Mat3 operator/(float s, const Mat3 &m)
{
    Mat3 r;
    for (int i = 0; i < 3; i++) r.cols[i] = m.cols[i] / s;
    return r;
}

const inline Mat3 Mat3::I = Mat3{Vec3{1.0f, 0.0f, 0.0f}, Vec3{0.0f, 1.0f, 0.0f},
                                 Vec3{0.0f, 0.0f, 1.0f}};
const inline Mat3 Mat3::Zero = Mat3{Vec3{0.0f, 0.0f, 0.0f}, Vec3{0.0f, 0.0f, 0.0f},
                                    Vec3{0.0f, 0.0f, 0.0f}};

inline Mat3 Mat3::transpose(const Mat3 &m)
{
    Mat3 r;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            r[i][j] = m[j][i];
        }
    }
    return r;
}

inline Mat3 Mat3::inverse(const Mat3 &m)
{
    // TODO:
    assert(false);
    return Mat3();
}

inline std::ostream &operator<<(std::ostream &out, Mat3 m)
{
    out << "{" << m[0] << "," << m[1] << "," << m[2] << "}";
    return out;
}

#endif //HINAPE_MAT3_H
