#ifndef HINAPE_ARRAY_ACCESSOR1_H
#define HINAPE_ARRAY_ACCESSOR1_H

#include "array_accessor.h"

#include "../parallel_lib/parallel_for.h"

namespace HinaPE
{
template<typename T>
class ArrayAccessor<T, 1> final
{
private:
    size_t _size;
    T *_data;

public:
    auto set(const ArrayAccessor &other) -> void;
    auto reset(size_t size, T *data) -> void;
    [[nodiscard]] auto at(size_t i) -> T &;
    [[nodiscard]] auto at(size_t i) const -> const T &;
    [[nodiscard]] auto begin() const -> T *;
    [[nodiscard]] auto end() const -> T *;
    [[nodiscard]] auto begin() -> T *;
    [[nodiscard]] auto end() -> T *;
    [[nodiscard]] auto size() const -> size_t;
    [[nodiscard]] auto data() const -> T *;
    auto swap(ArrayAccessor &other) -> void;

    template<typename Callback>
    auto forEach(Callback func) const -> void;
    template<typename Callback>
    auto forEachIndex(Callback func) -> void;
    template<typename Callback>
    auto parallelForEach(Callback func) -> void;
    template<typename Callback>
    auto parallelForEachIndex(Callback func) -> void;

public:
    ArrayAccessor();
    ArrayAccessor(size_t size, T *const data);
    ArrayAccessor(const ArrayAccessor &other);
    auto operator[](size_t i) -> T &;
    auto operator[](size_t i) const -> const T &;
    auto operator=(const ArrayAccessor &other) -> ArrayAccessor &;
    operator ConstArrayAccessor<T, 1>() const;
};

template<typename T>
ArrayAccessor<T, 1>::ArrayAccessor() : _size(0), _data(nullptr) {}
template<typename T>
ArrayAccessor<T, 1>::ArrayAccessor(size_t size, T *const data) : _size(size), _data(data) {}
template<typename T>
ArrayAccessor<T, 1>::ArrayAccessor(const ArrayAccessor &other) { set(other); }
template<typename T>
auto ArrayAccessor<T, 1>::set(const ArrayAccessor &other) -> void { reset(other._size, other._data); }
template<typename T>
auto ArrayAccessor<T, 1>::reset(size_t size, T *data) -> void
{
    _size = size;
    _data = data;
}
template<typename T>
auto ArrayAccessor<T, 1>::at(size_t i) -> T &
{
    assert(i < _size);
    return _data[i];
}
template<typename T>
auto ArrayAccessor<T, 1>::at(size_t i) const -> const T &
{
    assert(i < _size);
    return _data[i];
}
template<typename T>
auto ArrayAccessor<T, 1>::begin() const -> T * { return _data; }
template<typename T>
auto ArrayAccessor<T, 1>::end() const -> T * { return _data + _size; }
template<typename T>
auto ArrayAccessor<T, 1>::begin() -> T * { return _data; }
template<typename T>
auto ArrayAccessor<T, 1>::end() -> T * { return _data + _size; }
template<typename T>
auto ArrayAccessor<T, 1>::size() const -> size_t { return _size; }
template<typename T>
auto ArrayAccessor<T, 1>::data() const -> T * { return _data; }
template<typename T>
auto ArrayAccessor<T, 1>::swap(ArrayAccessor &other) -> void
{
    std::swap(other._data, _data);
    std::swap(other._size, _size);
}
template<typename T>
template<typename Callback>
auto ArrayAccessor<T, 1>::forEach(Callback func) const -> void
{
    for (size_t i = 0; i < size(); ++i)
        func(at(i));
}
template<typename T>
template<typename Callback>
auto ArrayAccessor<T, 1>::forEachIndex(Callback func) -> void
{
    for (size_t i = 0; i < size(); ++i)
        func(i);
}
template<typename T>
template<typename Callback>
auto ArrayAccessor<T, 1>::parallelForEach(Callback func) -> void
{
    parallel_for(0, size(), [&](size_t i) { func(at(i)); });
}
template<typename T>
template<typename Callback>
auto ArrayAccessor<T, 1>::parallelForEachIndex(Callback func) -> void
{
    parallel_for(0, size(), func);
}
template<typename T>
auto ArrayAccessor<T, 1>::operator[](size_t i) -> T & { return _data[i]; }
template<typename T>
auto ArrayAccessor<T, 1>::operator[](size_t i) const -> const T & { return _data[i]; }
template<typename T>
auto ArrayAccessor<T, 1>::operator=(const ArrayAccessor &other) -> ArrayAccessor &
{
    set(other);
    return *this;
}
template<typename T>
ArrayAccessor<T, 1>::operator ConstArrayAccessor<T, 1>() const { return ConstArrayAccessor<T, 1>(*this); }
template<typename T> using ArrayAccessor1 = ArrayAccessor<T, 1>;

template<typename T>
class ConstArrayAccessor<T, 1>
{
private:
    size_t _size;
    const T *_data;

public:
    [[nodiscard]] auto at(size_t i) const -> const T &;
    [[nodiscard]] auto begin() const -> const T *;
    [[nodiscard]] auto end() const -> const T *;
    [[nodiscard]] auto size() const -> size_t;
    [[nodiscard]] auto data() const -> const T *;

    template<typename Callback>
    void forEach(Callback func) const;
    template<typename Callback>
    void forEachIndex(Callback func);
    template<typename Callback>
    void parallelForEach(Callback func);
    template<typename Callback>
    void parallelForEachIndex(Callback func);

public:
    ConstArrayAccessor();
    ConstArrayAccessor(size_t size, const T *const data);
    explicit ConstArrayAccessor(const ArrayAccessor<T, 1> &other);
    ConstArrayAccessor(const ConstArrayAccessor &other);
    auto operator[](size_t i) const -> const T &;
};

template<typename T>
ConstArrayAccessor<T, 1>::ConstArrayAccessor() : _size(0), _data(nullptr) {}
template<typename T>
ConstArrayAccessor<T, 1>::ConstArrayAccessor(size_t size, const T *const data) : _size(size), _data(data) {}
template<typename T>
ConstArrayAccessor<T, 1>::ConstArrayAccessor(const ArrayAccessor<T, 1> &other)
{
    _size = other._size;
    _data = other._data;
}
template<typename T>
ConstArrayAccessor<T, 1>::ConstArrayAccessor(const ConstArrayAccessor &other)
{
    _size = other._size;
    _data = other._data;
}
template<typename T>
auto ConstArrayAccessor<T, 1>::at(size_t i) const -> const T &
{
    assert(i < _size);
    return _data[i];
}
template<typename T>
auto ConstArrayAccessor<T, 1>::begin() const -> const T * { return _data; }
template<typename T>
auto ConstArrayAccessor<T, 1>::end() const -> const T * { return _data + _size; }
template<typename T>
auto ConstArrayAccessor<T, 1>::size() const -> size_t { return _size; }
template<typename T>
auto ConstArrayAccessor<T, 1>::data() const -> const T * { return _data; }
template<typename T>
template<typename Callback>
void ConstArrayAccessor<T, 1>::forEach(Callback func) const
{
    for (size_t i = 0; i < size(); ++i)
        func(at(i));
}
template<typename T>
template<typename Callback>
void ConstArrayAccessor<T, 1>::forEachIndex(Callback func)
{
    for (size_t i = 0; i < size(); ++i)
        func(i);
}
template<typename T>
template<typename Callback>
void ConstArrayAccessor<T, 1>::parallelForEach(Callback func)
{
    // TODO: parallel for each
}
template<typename T>
template<typename Callback>
void ConstArrayAccessor<T, 1>::parallelForEachIndex(Callback func)
{
    // TODO: parallel for each
}
template<typename T>
auto ConstArrayAccessor<T, 1>::operator[](size_t i) const -> const T & { return _data[i]; }
template<typename T> using ConstArrayAccessor1 = ConstArrayAccessor<T, 1>;
}

#endif //HINAPE_ARRAY_ACCESSOR1_H
