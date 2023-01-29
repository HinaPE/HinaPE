#include "gtest/gtest.h"
#include "base/vector.h"
#include <iostream>

//#pragma pack (1)
struct Test1
{
	int d;
	int b;
	long a;
};
//#pragma pack(0)

// Demonstrate some basic assertions.
TEST(TestVector, Vector3f)
{
	EXPECT_EQ(sizeof(HinaPE::Base::Vector3<int>), 3 * sizeof(int));
	EXPECT_EQ(sizeof(HinaPE::Base::Vector3<float>), 3 * sizeof(float));
	EXPECT_EQ(sizeof(HinaPE::Base::Vector3<double>), 3 * sizeof(double));

	{
		HinaPE::Base::Vector3<float> v(1);
		EXPECT_EQ(v.x(), 1);
		EXPECT_EQ(v.y(), 1);
		EXPECT_EQ(v.z(), 1);
	}
	{
		HinaPE::Base::Vector3<float> v(1, 2, 3);
		EXPECT_EQ(v.x(), 1);
		EXPECT_EQ(v.y(), 2);
		EXPECT_EQ(v.z(), 3);
	}
	{
		HinaPE::Base::Vector3<float> v(1, 2, 3);
		HinaPE::Base::Vector3<float> v_copy(v);
		EXPECT_EQ(v_copy.x(), 1);
		EXPECT_EQ(v_copy.y(), 2);
		EXPECT_EQ(v_copy.z(), 3);
	}
	{
		HinaPE::Base::Vector3<float> v(1, 2, 3);
		HinaPE::Base::Vector3<float> v_move(std::move(v));
		EXPECT_EQ(v_move.x(), 1);
		EXPECT_EQ(v_move.y(), 2);
		EXPECT_EQ(v_move.z(), 3);
	}
	{
		HinaPE::Base::Vector3<float> v1(1, 2, 3);
		HinaPE::Base::Vector3<float> v2(2, 3, 4);

		v1 += 1;
		EXPECT_EQ(v1.x(), 2);
		EXPECT_EQ(v1.y(), 3);
		EXPECT_EQ(v1.z(), 4);

		v1 -= 1;
		EXPECT_EQ(v1.x(), 1);
		EXPECT_EQ(v1.y(), 2);
		EXPECT_EQ(v1.z(), 3);

		v1 *= 2;
		EXPECT_EQ(v1.x(), 2);
		EXPECT_EQ(v1.y(), 4);
		EXPECT_EQ(v1.z(), 6);

		v1 /= 2;
		EXPECT_EQ(v1.x(), 1);
		EXPECT_EQ(v1.y(), 2);
		EXPECT_EQ(v1.z(), 3);

		v1 += v2;
		EXPECT_EQ(v1.x(), 3);
		EXPECT_EQ(v1.y(), 5);
		EXPECT_EQ(v1.z(), 7);

		v1 -= v2;
		EXPECT_EQ(v1.x(), 1);
		EXPECT_EQ(v1.y(), 2);
		EXPECT_EQ(v1.z(), 3);

		v1 *= v2;
		EXPECT_EQ(v1.x(), 2);
		EXPECT_EQ(v1.y(), 6);
		EXPECT_EQ(v1.z(), 12);

		v1 /= v2;
		EXPECT_EQ(v1.x(), 1);
		EXPECT_EQ(v1.y(), 2);
		EXPECT_EQ(v1.z(), 3);
	}
	{
		HinaPE::Base::Vector3<float> v1(1, 2, 3);
		HinaPE::Base::Vector3<float> v2(2, 3, 4);
		auto res = v1.dot(v2);
		EXPECT_EQ(res, 20);
	}
	{
		HinaPE::Base::Vector3<float> v1(1, 2, 3);
		HinaPE::Base::Vector3<float> v2(2, 3, 4);
		auto res = v1.cross(v2);
		EXPECT_EQ(res.x(), -1);
		EXPECT_EQ(res.y(), 2);
		EXPECT_EQ(res.z(), -1);
	}
}

auto main(int argc, char **argv) -> int
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}