#ifndef HINAPE_ARRAY1_H
#define HINAPE_ARRAY1_H

#include "array.h"

#include <vector>

namespace HinaPE
{
template<typename T>
class Array<T, 1> final
{
    using ContainerType = std::vector<T>;
    using Iterator = typename ContainerType::iterator;
    using ConstIterator = typename ContainerType::const_iterator;
public:
    void set(const T& value);
    void set(const Array& other);
    void set(const std::initializer_list<std::initializer_list<std::initializer_list<T>>>& lst);
    void clear();
//    void resize(const Size3& size, const T& initVal = T());

public:
    Array() = default;
    explicit Array(size_t size, const T &init_val = T());
    explicit Array(size_t width, size_t height, size_t depth, const T& initVal = T());
    Array(const std::initializer_list<std::initializer_list<std::initializer_list<T>>>& lst);
    Array(const Array& other);
    Array(Array&& other) noexcept ;
};

template<typename T>
Array<T, 1>::Array(size_t size, const T &init_val)
{

}
template<typename T>
Array<T, 1>::Array(size_t width, size_t height, size_t depth, const T &initVal)
{

}
template<typename T>
Array<T, 1>::Array(const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &lst)
{

}
template<typename T>
Array<T, 1>::Array(const Array &other)
{

}
template<typename T>
Array<T, 1>::Array(Array &&other) noexcept
{

}
}

#endif //HINAPE_ARRAY1_H
