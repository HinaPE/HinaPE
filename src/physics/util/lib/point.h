#ifndef HINAPE_POINT_H
#define HINAPE_POINT_H

#include "lib/log.h"

#include <array>
#include <type_traits>

namespace HinaPE
{
template<typename T, size_t N>
class Point final
{
public:
    template<typename U>
    auto set(const std::initializer_list<U> &lst) -> void;
    auto set(const Point &other) -> void;
    template<typename U>
    auto operator=(const std::initializer_list<U> &lst) -> Point &;
    auto operator=(const Point &other) -> Point &;
    auto operator[](size_t i) const -> const T &;
    auto operator[](size_t i) -> T &;

public:
    Point() = default;
    template<typename... Params>
    explicit Point(Params... params);
    template<typename U>
    Point(const std::initializer_list<U> &lst);
    Point(const Point &other);

private:
    template<typename... Params>
    auto set_at(size_t i, T v, Params... params) -> void;
    auto set_at(size_t i, T v) -> void;

    std::array<T, N> _elements;

public:
    static_assert(N > 0, "Size of static-sized point should be greater than zero.");
    static_assert(std::is_arithmetic<T>::value, "Point only can be instantiated with arithmetic types");
};

template<typename T, size_t N>
template<typename... Params>
Point<T, N>::Point(Params... params)
{
    static_assert(sizeof...(params) == N, "Invalid number of parameters.");
    setAt(0, params...);
}
template<typename T, size_t N>
template<typename U>
Point<T, N>::Point(const std::initializer_list<U> &lst)
{
    assert(lst.size() >= N);

    size_t i = 0;
    for (const auto &input_elem: lst)
    {
        _elements[i] = static_cast<T>(input_elem);
        ++i;
    }
}
template<typename T, size_t N>
Point<T, N>::Point(const Point &other) : _elements(other._elements) {}
template<typename T, size_t N>
template<typename U>
void Point<T, N>::set(const std::initializer_list<U> &lst)
{
    assert(lst.size() >= N);

    size_t i = 0;
    for (const auto &inputElem: lst)
    {
        _elements[i] = static_cast<T>(inputElem);
        ++i;
    }
}
template<typename T, size_t N>
auto Point<T, N>::set(const Point &other) -> void { _elements = other._elements; }
template<typename T, size_t N>
template<typename U>
auto Point<T, N>::operator=(const std::initializer_list<U> &lst) -> Point<T, N> &
{
    set(lst);
    return *this;
}
template<typename T, size_t N>
auto Point<T, N>::operator=(const Point &other) -> Point<T, N> &
{
    set(other);
    return *this;
}
template<typename T, size_t N>
auto Point<T, N>::operator[](size_t i) const -> const T &{ return _elements[i]; }
template<typename T, size_t N>
auto Point<T, N>::operator[](size_t i) -> T & { return _elements[i]; }
template<typename T, size_t N>
template<typename... Params>
auto Point<T, N>::set_at(size_t i, T v, Params... params) -> void
{
    _elements[i] = v;
    set_at(i + 1, params...);
}
template<typename T, size_t N>
auto Point<T, N>::set_at(size_t i, T v) -> void { _elements[i] = v; }
}

#endif //HINAPE_POINT_H
