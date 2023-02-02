#ifndef HINAPE_TEST_PARALLEL_H
#define HINAPE_TEST_PARALLEL_H
#include "gtest/gtest.h"

#include "util/parallel.h"
#include "util/timer.h"

#include <cstdlib>
#include <iostream>
#include <future>
#include <thread>
#include <ctime>

using namespace HinaPE::Util;

TEST(TestParallel, Parallel)
{
	int scale = 2000;
	std::srand(std::time(nullptr));
	std::vector<int> v1;
	for (int i = 0; i < scale; ++i)
		v1.push_back(std::rand());

	auto f1 = std::async(std::launch::async,
						 [&]() -> float
						 {
							 Timer timer;

							 for (int i = 0; i < scale; ++i)
							 {
								 auto v2 = v1;
								 std::sort(v2.begin(), v2.end());
							 }

							 timer.duration("For");
							 return timer._last_duration;
						 });

	auto f2 = std::async(std::launch::async,
						 [&]() -> float
						 {
							 Timer timer;

							 parallelFor(0, scale,
													 [&](int i)
													 {
														 auto v2 = v1;
														 std::sort(v2.begin(), v2.end());
													 },
													 ExecutionPolicy::Parallel);

							 timer.duration("parallelFor");
							 return timer._last_duration;
						 });

	std::cout << "acceleration rate: " << f1.get() / f2.get() << std::endl;
	std::cout << "thread num: " << std::thread::hardware_concurrency() << std::endl;
}

#endif //HINAPE_TEST_PARALLEL_H
