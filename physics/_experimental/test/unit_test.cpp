#include "test_parallel.h"
#include "test_math.h"

int Timer::stack_depth = 0;

auto main(int argc, char **argv) -> int
{
	// run unit test first
	::testing::InitGoogleTest(&argc, argv);
	bool res = RUN_ALL_TESTS();

	// run sim application

	return res;
}