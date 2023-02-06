#include "test_parallel.h"
#include "test_math.h"
#include "test_quaternion.h"

auto main(int argc, char **argv) -> int
{
	// run unit test first
	::testing::InitGoogleTest(&argc, argv);
	bool res = RUN_ALL_TESTS();

	return res;
}