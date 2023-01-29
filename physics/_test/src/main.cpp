#include <iostream>

#include "gtest/gtest.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
	std::cout << "Hello, World!" << std::endl;
}


// Demonstrate some basic assertions.
TEST(HelloTest2, BasicAssertions2) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
	std::cout << "Hello, World!" << std::endl;
}

auto main(int argc, char **argv) -> int {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}