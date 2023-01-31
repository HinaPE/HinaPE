#ifndef HINAPE_PARALLEL_H
#define HINAPE_PARALLEL_H

namespace Hina::Base
{
enum class ExecutionPolicy { Serial, Parallel };
/*
template<typename RandomIterator, typename T>
void parallelFill(const RandomIterator &begin, const RandomIterator &end, const T &value, ExecutionPolicy policy = ExecutionPolicy::Parallel);

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

template<typename IndexType, typename Value, typename Function, typename Reduce>
auto parallelReduce(IndexType begin_index, IndexType end_index, const Value &identity, const Function &func, const Reduce &reduce, ExecutionPolicy policy = ExecutionPolicy::Parallel) -> Value;

template<typename RandomIterator>
void parallelSort(RandomIterator begin, RandomIterator end, ExecutionPolicy policy = ExecutionPolicy::Parallel);

template<typename RandomIterator, typename CompareFunction>
void parallelSort(RandomIterator begin, RandomIterator end, CompareFunction compare, ExecutionPolicy policy = ExecutionPolicy::Parallel);

void set_max_number_of_threads(unsigned int num_threads);

auto max_number_of_threads() -> unsigned int;
*/
}

#ifdef HINAPE_TBB
#include <tbb/parallel_for.h>
//#include <tbb/parallel_reduce.h>
#include <tbb/parallel_sort.h>
#include <tbb/task.h>

namespace Hina::Base
{
template<typename IndexType, typename Function>
void parallelFor(IndexType begin_index, IndexType end_index, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel)
{
	if (begin_index > end_index)
		return;

	if (policy == ExecutionPolicy::Parallel)
		tbb::parallel_for(begin_index, end_index, function);
	else
		for (auto i = begin_index; i < end_index; ++i)
			function(i);
}

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_index, IndexType end_index, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel)
{
	if (begin_index > end_index)
		return;

	if (policy == ExecutionPolicy::Parallel)
		tbb::parallel_for(tbb::blocked_range<IndexType>(begin_index, end_index),
						  [&function](const tbb::blocked_range<IndexType> &range)
						  {
							  function(range.begin(), range.end());
						  });
	else
		func(begin_index, end_index);
}

template<typename IndexType, typename Function>
void parallelFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel)
{
	parallelFor(begin_indexY, end_indexY,
				[&](IndexType j)
				{
					for (IndexType i = begin_indexX; i < end_indexX; ++i)
						function(i, j);
				},
				policy);
}

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel)
{
	parallelRangeFor(begin_indexY, end_indexY,
					 [&](IndexType jBegin, IndexType jEnd)
					 {
						 function(begin_indexX, end_indexX, jBegin, jEnd);
					 },
					 policy);
}

template<typename IndexType, typename Function>
void parallelFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, IndexType begin_indexZ, IndexType end_indexZ, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel)
{
	parallelFor(begin_indexZ, end_indexZ,
				[&](IndexType k)
				{
					for (IndexType j = begin_indexY; j < end_indexY; ++j)
						for (IndexType i = begin_indexX; i < end_indexX; ++i)
							function(i, j, k);
				},
				policy);
}

template<typename IndexType, typename Function>
void parallelRangeFor(IndexType begin_indexX, IndexType end_indexX, IndexType begin_indexY, IndexType end_indexY, IndexType begin_indexZ, IndexType end_indexZ, const Function &function, ExecutionPolicy policy = ExecutionPolicy::Parallel)
{
	parallelRangeFor(begin_indexZ, end_indexZ,
					 [&](IndexType kBegin, IndexType kEnd)
					 {
						 function(begin_indexX, end_indexX, begin_indexY, end_indexY, kBegin, kEnd);
					 },
					 policy);
}
}
#endif
#endif //HINAPE_PARALLEL_H
