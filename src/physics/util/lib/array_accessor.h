#ifndef HINAPE_ARRAY_ACCESSOR_H
#define HINAPE_ARRAY_ACCESSOR_H

#include "lib/log.h"

namespace HinaPE
{
template<typename T, size_t N>
class ArrayAccessor final
{
public:
    static_assert(N < 1 || N > 3, "Not implemented - N should be either 1, 2 or 3.");
};

template<typename T, size_t N>
class ConstArrayAccessor final
{
public:
    static_assert(N < 1 || N > 3, "Not implemented - N should be either 1, 2 or 3.");
};
}

#endif //HINAPE_ARRAY_ACCESSOR_H
