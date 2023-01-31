#ifndef HINAPE_PARALLEL_H
#define HINAPE_PARALLEL_H

namespace Hina::Base
{
enum class ExecutionPolicy { Serial, Parallel };

template<typename RandomIterator, typename T>
void parallelFill(const RandomIterator &begin, const RandomIterator &end, const T &value, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelFor(IndexType beginIndex, IndexType endIndex, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType beginIndex, IndexType endIndex, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelFor(IndexType beginIndexX, IndexType endIndexX, IndexType beginIndexY, IndexType endIndexY, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType beginIndexX, IndexType endIndexX, IndexType beginIndexY, IndexType endIndexY, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelFor(IndexType beginIndexX, IndexType endIndexX, IndexType beginIndexY, IndexType endIndexY, IndexType beginIndexZ, IndexType endIndexZ, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType beginIndexX, IndexType endIndexX, IndexType beginIndexY, IndexType endIndexY, IndexType beginIndexZ, IndexType endIndexZ, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename IndexType, typename Value, typename Function, typename Reduce>
auto parallelReduce(IndexType beginIndex, IndexType endIndex, const Value &identity, const Function &func, const Reduce &reduce, ExecutionPolicy policy = ExecutionPolicy::Parallel) -> Value;

template<typename RandomIterator>
void parallelSort(RandomIterator begin, RandomIterator end, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename RandomIterator, typename CompareFunction>
void parallelSort(RandomIterator begin, RandomIterator end, CompareFunction compare, ExecutionPolicy policy = ExecutionPolicy::Parallel);

void set_max_number_of_threads(unsigned int numThreads);

auto max_number_of_threads() -> unsigned int;

#ifdef HINAPE_TBB
#include "impl/parallel_impl_tbb.h"
#endif
}

#endif //HINAPE_PARALLEL_H
