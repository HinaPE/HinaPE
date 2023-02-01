#include "test_parallel.h"
#include "test_math.h"

#include "grid-based/solver.h"

auto main(int argc, char **argv) -> int
{
	// run unit test first
	::testing::InitGoogleTest(&argc, argv);
	bool res = RUN_ALL_TESTS();

	// run sim application
	auto solver = std::make_shared<Hina::GridFluidSolver>(Hina::Base::Size3(32, 32, 32), Hina::mVector3(1, 1, 1), Hina::mVector3(0, 0, 0));

	return res;
}