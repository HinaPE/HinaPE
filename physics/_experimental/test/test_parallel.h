#ifndef HINAPE_TEST_PARALLEL_H
#define HINAPE_TEST_PARALLEL_H
#include "gtest/gtest.h"

#include "base/parallel.h"
#include "base/timer.h"

#include <cstdlib>
#include <iostream>
#include <future>
#include <thread>
#include <ctime>

TEST(TestParallel, Parallel)
{
	int scale = 10000;
	std::srand(std::time(nullptr));
	std::vector<int> v1;
	for (int i = 0; i < scale; ++i)
		v1.push_back(std::rand());

	auto f1 = std::async(std::launch::async,
						 [&]()
						 {
							 Timer timer;

							 long long sum = 0;
							 for (int i = 0; i < scale; ++i)
							 {
								 auto v2 = v1;
								 std::sort(v2.begin(), v2.end());
								 sum += v2[i];
							 }
							 std::cout << "sum = " << sum << std::endl;

							 timer.duration("For");
						 });

	auto f2 = std::async(std::launch::async,
						 [&]()
						 {
							 Timer timer;

							 {
								 long long sum = 0;
								 Hina::Base::parallelFor(0, scale,
														 [&](int i)
														 {
															 auto v2 = v1;
															 std::sort(v2.begin(), v2.end());
															 sum += v2[i];
														 },
														 Hina::Base::ExecutionPolicy::Parallel);
								 std::cout << "sum = " << sum << std::endl;
							 }

							 timer.duration("parallelFor");
						 });

	f1.get();
	f2.get();
}

#endif //HINAPE_TEST_PARALLEL_H
