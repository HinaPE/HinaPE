#ifndef HINAPE_PARALLEL_IMPL_CPP_H
#define HINAPE_PARALLEL_IMPL_CPP_H

#include <thread>

#include "../parallel.h"

namespace HinaPE::Util
{
template<typename IndexType, typename Function>
void parallelFor(IndexType begin_index, IndexType end_index, const Function &function, ExecutionPolicy policy) {}

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_index, IndexType end_index, const Function &function, ExecutionPolicy policy) {}

template<typename IndexType, typename Function>
void parallelFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, const Function &function, ExecutionPolicy policy) {}

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, const Function &function, ExecutionPolicy policy) {}

template<typename IndexType, typename Function>
void parallelFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, IndexType begin_indexZ, IndexType end_indexZ, const Function &function, ExecutionPolicy policy) {}

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, IndexType begin_indexZ, IndexType end_indexZ, const Function &function, ExecutionPolicy policy) {}
}
#endif //HINAPE_PARALLEL_IMPL_CPP_H
