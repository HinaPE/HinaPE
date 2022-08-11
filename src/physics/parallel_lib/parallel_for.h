#ifndef HINAPE_PARALLEL_FOR_H
#define HINAPE_PARALLEL_FOR_H

#include "tbb/parallel_for.h"
#include <thread>

namespace HinaPE
{
enum class ExecutionPolicy { kSerial, kParallel };

template<typename IndexType, typename Function>
void parallel_for(IndexType start, IndexType end, const Function &func, ExecutionPolicy policy)
{
#ifdef HINAPE_TBB_SUPPORT
    tbb::parallel_for(start, end, func);

    tbb::parallel_for(tbb::blocked_range<IndexType>(start, end), [&](tbb::blocked_range<IndexType> r)
    {
        for (IndexType i = r.begin(); i != r.end(); ++i)
        {
            func(i);
        }
    });
#endif
}
}

#endif //HINAPE_PARALLEL_FOR_H
