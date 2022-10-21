#ifndef INCLUDE_JET_DETAIL_BOUNDING_BOX_INL_H_
#define INCLUDE_JET_DETAIL_BOUNDING_BOX_INL_H_

#include "bounding_box.h"

#include "math_utils.h"

#include <algorithm>
#include <limits>

namespace jet
{

template<typename T, size_t N>
BoundingBox<T, N>::BoundingBox()
{
    reset();
}

template<typename T, size_t N>
BoundingBox<T, N>::BoundingBox(const VectorType &point1, const VectorType &point2)
{
    for (size_t i = 0; i < N; ++i)
    {
        lowerCorner[i] = std::min(point1[i], point2[i]);
        upperCorner[i] = std::max(point1[i], point2[i]);
    }
}

template<typename T, size_t N>
BoundingBox<T, N>::BoundingBox(const BoundingBox &other) :
        lowerCorner(other.lowerCorner), upperCorner(other.upperCorner)
{
}

template<typename T, size_t N>
auto BoundingBox<T, N>::overlaps(const BoundingBox &other) const -> bool
{
    for (size_t i = 0; i < N; ++i)
    {
        if (upperCorner[i] < other.lowerCorner[i] || lowerCorner[i] > other.upperCorner[i])
        {
            return false;
        }
    }

    return true;
}

template<typename T, size_t N>
auto BoundingBox<T, N>::contains(const VectorType &point) const -> bool
{
    for (size_t i = 0; i < N; ++i)
    {
        if (upperCorner[i] < point[i] || lowerCorner[i] > point[i])
        {
            return false;
        }
    }

    return true;
}

template<typename T, size_t N>
auto BoundingBox<T, N>::midPoint() const -> Vector<T, N>
{
    Vector<T, N> result;
    for (size_t i = 0; i < N; ++i)
    {
        result[i] = (upperCorner[i] + lowerCorner[i]) / 2;
    }
    return result;
}

template<typename T, size_t N>
auto BoundingBox<T, N>::diagonalLength() const -> T
{
    T result = 0;
    for (size_t i = 0; i < N; ++i)
    {
        result += square(upperCorner[i] - lowerCorner[i]);
    }
    return std::sqrt(result);
}

template<typename T, size_t N>
auto BoundingBox<T, N>::diagonalLengthSquared() const -> T
{
    T result = 0;
    for (size_t i = 0; i < N; ++i)
    {
        result += square(upperCorner[i] - lowerCorner[i]);
    }
    return result;
}

template<typename T, size_t N>
void BoundingBox<T, N>::reset()
{
    for (size_t i = 0; i < N; ++i)
    {
        lowerCorner[i] = std::numeric_limits<T>::max();
        upperCorner[i] = -std::numeric_limits<T>::max();
    }
}

template<typename T, size_t N>
void BoundingBox<T, N>::merge(const VectorType &point)
{
    for (size_t i = 0; i < N; ++i)
    {
        lowerCorner[i] = std::min(lowerCorner[i], point[i]);
        upperCorner[i] = std::max(upperCorner[i], point[i]);
    }
}

template<typename T, size_t N>
void BoundingBox<T, N>::merge(const BoundingBox &other)
{
    for (size_t i = 0; i < N; ++i)
    {
        lowerCorner[i] = std::min(lowerCorner[i], other.lowerCorner[i]);
        upperCorner[i] = std::max(upperCorner[i], other.upperCorner[i]);
    }
}

template<typename T, size_t N>
void BoundingBox<T, N>::expand(T delta)
{
    for (size_t i = 0; i < N; ++i)
    {
        lowerCorner[i] -= delta;
        upperCorner[i] += delta;
    }
}

}  // namespace jet

#endif  // INCLUDE_JET_DETAIL_BOUNDING_BOX_INL_H_
