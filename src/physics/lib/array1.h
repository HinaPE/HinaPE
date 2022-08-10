#ifndef HINAPE_ARRAY1_H
#define HINAPE_ARRAY1_H

#include "array.h"
#include "array_accessor1.h"

#include <vector>

namespace HinaPE
{
template<typename T>
class Array<T, 1> final
{
    using ContainerType = std::vector<T>;
    using Iterator = typename ContainerType::iterator;
    using ConstIterator = typename ContainerType::const_iterator;
private:
    ContainerType _data;

public:
    auto set(const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &lst) -> void;
    auto set(const T &value) -> void;
    auto set(const Array &other) -> void;
    auto clear() -> void;
    auto resize(size_t size, const T &init_val = T()) -> void;
    [[nodiscard]] auto at(size_t i) -> T &;
    [[nodiscard]] auto at(size_t i) const -> const T &;
    [[nodiscard]] auto size() const -> size_t;
    [[nodiscard]] auto data() -> T *;
    [[nodiscard]] auto data() const -> const T *;
    [[nodiscard]] auto begin() -> Iterator;
    [[nodiscard]] auto begin() const -> ConstIterator;
    [[nodiscard]] auto end() -> Iterator;
    [[nodiscard]] auto end() const -> ConstIterator;
    [[nodiscard]] auto accessor() -> ArrayAccessor1<T>;
    [[nodiscard]] auto const_accessor() const -> ConstArrayAccessor1<T>;
    auto swap(Array &other) -> void;
    auto append(const T &newVal) -> void;
    auto append(const Array &other) -> void;

    template<typename Callback>
    void forEach(Callback func) const;
    template<typename Callback>
    void forEachIndex(Callback func) const;
    template<typename Callback>
    void parallelForEach(Callback func);
    template<typename Callback>
    void parallelForEachIndex(Callback func) const;

public:
    Array();
    explicit Array(size_t size, const T &init_val = T());
    Array(const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &lst);
    Array(const Array &other);
    Array(Array &&other) noexcept;

    auto operator=(const std::initializer_list<T> &lst) -> Array &;
};

template<typename T>
Array<T, 1>::Array() = default;
template<typename T>
Array<T, 1>::Array(size_t size, const T &init_val) { resize(size, init_val); }
template<typename T>
Array<T, 1>::Array(const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &lst) { set(lst); }
template<typename T>
Array<T, 1>::Array(const Array &other) { set(other); }
template<typename T>
Array<T, 1>::Array(Array &&other) noexcept { (*this) = std::move(other); }
template<typename T>
auto Array<T, 1>::set(const std::initializer_list<std::initializer_list<std::initializer_list<T>>> &lst) -> void
{
    size_t size = lst.size();
    resize(size);
    auto col_iter = lst.begin();
    for (size_t i = 0; i < size; ++i)
    {
        (*this)[i] = *col_iter;
        ++col_iter;
    }
}
template<typename T>
auto Array<T, 1>::set(const T &value) -> void
{
    for (auto &v: _data)
        v = value;
}
template<typename T>
auto Array<T, 1>::set(const Array &other) -> void
{
    _data.resize(other._data.size());
    std::copy(other._data.begin(), other._data.end(), _data.begin());
}
template<typename T>
auto Array<T, 1>::clear() -> void { _data.clear(); }
template<typename T>
auto Array<T, 1>::resize(size_t size, const T &init_val) -> void { _data.resize(size, init_val); }
template<typename T>
auto Array<T, 1>::at(size_t i) -> T &
{
    assert(i < size());
    return _data[i];
}
template<typename T>
auto Array<T, 1>::at(size_t i) const -> const T &
{
    assert(i < size());
    return _data[i];
}
template<typename T>
auto Array<T, 1>::size() const -> size_t { return _data.size(); }
template<typename T>
auto Array<T, 1>::data() -> T * { return _data.data(); }
template<typename T>
auto Array<T, 1>::data() const -> const T * { return _data.data(); }
template<typename T>
auto Array<T, 1>::begin() -> Iterator { return _data.begin(); }
template<typename T>
auto Array<T, 1>::begin() const -> ConstIterator { return _data.cbegin(); }
template<typename T>
auto Array<T, 1>::end() -> Iterator { return _data.end(); }
template<typename T>
auto Array<T, 1>::end() const -> ConstIterator { return _data.cend(); }
template<typename T>
auto Array<T, 1>::accessor() -> ArrayAccessor1<T> { return ArrayAccessor1<T>(size(), data()); }
template<typename T>
auto Array<T, 1>::const_accessor() const -> ConstArrayAccessor1<T> { return ConstArrayAccessor1<T>(size(), data()); }
template<typename T>
auto Array<T, 1>::swap(Array &other) -> void { std::swap(other._data, _data); }
template<typename T>
auto Array<T, 1>::append(const T &new_val) -> void { _data.push_back(new_val); }
template<typename T>
auto Array<T, 1>::append(const Array &other) -> void { _data.insert(_data.end(), other._data.begin(), other._data.end()); }
template<typename T>
template<typename Callback>
void Array<T, 1>::forEach(Callback func) const { const_accessor().forEach(func); }
template<typename T>
template<typename Callback>
void Array<T, 1>::forEachIndex(Callback func) const { const_accessor().forEachIndex(func); }
template<typename T>
template<typename Callback>
void Array<T, 1>::parallelForEach(Callback func) { accessor().parallelForEach(func); /* TODO: why here is accessor? */ }
template<typename T>
template<typename Callback>
void Array<T, 1>::parallelForEachIndex(Callback func) const { const_accessor().parallelForEachIndex(func); }

template<typename T>
auto Array<T, 1>::operator=(const std::initializer_list<T> &lst) -> Array<T, 1> &
{
    set(lst);
    return *this;
}

template<typename T> using Array1 = Array<T, 1>;
}

#endif //HINAPE_ARRAY1_H
