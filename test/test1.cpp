#include "test1.h"

#include "lib/mathlib.h"
#include "vector"

#include <thread>
#include <iostream>

#ifdef HINAPE_TBB_SUPPORT
#include "tbb/parallel_for.h"
#endif

void HinaPE::TEST::Test1::test()
{
    std::atomic<int> counter = 0;
    std::thread t1([&]()
                   {
                       for (int i = 0; i < 10000; i++)
                       {
                           counter++;
                       }
                   });
    std::thread t2([&]()
                   {
                       for (int i = 0; i < 10000; i++)
                       {
                           counter++;
                       }
                   });
    t1.join();
    t2.join();
    std::cout << counter << std::endl;
}
