#ifndef HINAPE_TEST_MATH_H
#define HINAPE_TEST_MATH_H
#include "gtest/gtest.h"

#include "base/vector.h"
#include "base/matrix.h"

#include <iostream>

// Demonstrate some basic assertions.
TEST(TestVector, Vector3f)
{
	EXPECT_EQ(sizeof(Hina::Base::Vector3<char>), 3 * sizeof(char));
	EXPECT_EQ(sizeof(Hina::Base::Vector3<short>), 3 * sizeof(short));
	EXPECT_EQ(sizeof(Hina::Base::Vector3<int>), 3 * sizeof(int));
	EXPECT_EQ(sizeof(Hina::Base::Vector3<float>), 3 * sizeof(float));
	EXPECT_EQ(sizeof(Hina::Base::Vector3<double>), 3 * sizeof(double));

	{ // constructor
		Hina::Base::Vector3<float> v(1);
		EXPECT_EQ(v.x(), 1);
		EXPECT_EQ(v.y(), 1);
		EXPECT_EQ(v.z(), 1);
	}
	{ // constructor
		Hina::Base::Vector3<float> v(1, 2, 3);
		EXPECT_EQ(v.x(), 1);
		EXPECT_EQ(v.y(), 2);
		EXPECT_EQ(v.z(), 3);
	}
	{ // copy constructor
		Hina::Base::Vector3<float> v(1, 2, 3);
		Hina::Base::Vector3<float> v_copy(v);
		EXPECT_EQ(v_copy.x(), 1);
		EXPECT_EQ(v_copy.y(), 2);
		EXPECT_EQ(v_copy.z(), 3);
	}
	{ // move constructor
		Hina::Base::Vector3<float> v(1, 2, 3);
		Hina::Base::Vector3<float> v_move(std::move(v));
		EXPECT_EQ(v_move.x(), 1);
		EXPECT_EQ(v_move.y(), 2);
		EXPECT_EQ(v_move.z(), 3);
	}
	{ // += -= *= /= operator
		Hina::Base::Vector3<float> v1(1, 2, 3);
		Hina::Base::Vector3<float> v2(2, 3, 4);

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
	{ // dot
		Hina::Base::Vector3<float> v1(1, 2, 3);
		Hina::Base::Vector3<float> v2(2, 3, 4);
		auto res = v1.dot(v2);
		EXPECT_EQ(res, 20);
	}
	{ // cross
		Hina::Base::Vector3<float> v1(1, 2, 3);
		Hina::Base::Vector3<float> v2(2, 3, 4);
		auto res = v1.cross(v2);
		EXPECT_EQ(res.x(), -1);
		EXPECT_EQ(res.y(), 2);
		EXPECT_EQ(res.z(), -1);
	}
}

TEST(TestMatrix, Matrix3x3f)
{
	EXPECT_EQ(sizeof(Hina::Base::Matrix3x3<char>), 9 * sizeof(char));
	EXPECT_EQ(sizeof(Hina::Base::Matrix3x3<short>), 9 * sizeof(short));
	EXPECT_EQ(sizeof(Hina::Base::Matrix3x3<int>), 9 * sizeof(int));
	EXPECT_EQ(sizeof(Hina::Base::Matrix3x3<float>), 9 * sizeof(float));
	EXPECT_EQ(sizeof(Hina::Base::Matrix3x3<double>), 9 * sizeof(double));

	{
		EXPECT_EQ(Hina::Base::Matrix3x3<float>::Zero()._m, Eigen::Matrix3f::Zero());
		EXPECT_EQ(Hina::Base::Matrix3x3<float>::Identity()._m, Eigen::Matrix3f::Identity());
		EXPECT_EQ(Hina::Base::Matrix3x3<float>()._m, Eigen::Matrix3f::Zero());
	}
	{
		Eigen::Matrix3f inv;
		inv << -5, 3, 4, 7, 3, -8, 1, -3, 4;
		inv /= 12.f;
		std::cout << Hina::Base::Matrix3x3<float>({1, 2, 3, 3, 2, 1, 2, 1, 3}).inverse()._m << std::endl;
		std::cout << inv << std::endl;
//		EXPECT_EQ(, inv);
	}
}

#endif //HINAPE_TEST_MATH_H
