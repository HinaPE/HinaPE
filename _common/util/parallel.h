#ifndef HINAPE_PARALLEL_H
#define HINAPE_PARALLEL_H

namespace HinaPE::Util
{
enum class ExecutionPolicy { Serial, Parallel };

template<typename IndexType, typename Function>
void parallelFor(IndexType begin_index, IndexType end_index, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_index, IndexType end_index, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, IndexType begin_indexZ, IndexType end_indexZ, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, IndexType begin_indexZ, IndexType end_indexZ, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename RandomIterator, typename T>
void parallelFill(const RandomIterator &begin, const RandomIterator &end, const T &value, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Value, typename Function, typename Reduce>
auto parallelReduce(IndexType begin_index, IndexType end_index, const Value &identity, const Function &func, const Reduce &reduce, ExecutionPolicy policy = ExecutionPolicy::Parallel) -> Value;

template<typename RandomIterator>
void parallelSort(RandomIterator begin, RandomIterator end, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename RandomIterator, typename CompareFunction>
void parallelSort(RandomIterator begin, RandomIterator end, CompareFunction compare, ExecutionPolicy policy = ExecutionPolicy::Parallel);

void set_max_number_of_threads(unsigned int num_threads);

auto max_number_of_threads() -> unsigned int;
}
#ifdef HINAPE_TBB
#include "impl/parallel_impl_tbb.h"
#else
#include "impl/parallel_impl_cpp.h"
#endif
#endif //HINAPE_PARALLEL_H
