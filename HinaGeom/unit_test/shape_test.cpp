#include <gtest/gtest.h>

#include "simple_shapes/sphere.h"

// Demonstrate some basic assertions.
TEST(ShapeTest, BasicAssertions)
{
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);

    std::cout << "Test Shape Start" << std::endl;

    auto sphere = HinaPE::HinaGeom::Sphere::build_Sphere(1);

    std::cout << "Test Shape End" << std::endl;
}
