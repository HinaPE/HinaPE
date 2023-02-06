#ifndef HINAPE_TEST_MATH_H
#define HINAPE_TEST_MATH_H
#include "gtest/gtest.h"

#include "math/vector.h"
#include "math/matrix.h"

using namespace HinaPE::Math;

// Demonstrate some basic assertions.
TEST(TestVector, Vector3f)
{
	EXPECT_EQ(sizeof(Vector3<char>), 3 * sizeof(char));
	EXPECT_EQ(sizeof(Vector3<short>), 3 * sizeof(short));
	EXPECT_EQ(sizeof(Vector3<int>), 3 * sizeof(int));
	EXPECT_EQ(sizeof(Vector3<float>), 3 * sizeof(float));
	EXPECT_EQ(sizeof(Vector3<double>), 3 * sizeof(double));

	{ // constructor
		Vector3<float> v(1);
		EXPECT_EQ(v.x(), 1);
		EXPECT_EQ(v.y(), 1);
		EXPECT_EQ(v.z(), 1);
	}
	{ // constructor
		Vector3<float> v(1, 2, 3);
		EXPECT_EQ(v.x(), 1);
		EXPECT_EQ(v.y(), 2);
		EXPECT_EQ(v.z(), 3);
	}
	{ // copy constructor
		Vector3<float> v(1, 2, 3);
		Vector3<float> v_copy(v);
		EXPECT_EQ(v_copy.x(), 1);
		EXPECT_EQ(v_copy.y(), 2);
		EXPECT_EQ(v_copy.z(), 3);
	}
	{ // move constructor
		Vector3<float> v(1, 2, 3);
		Vector3<float> v_move(std::move(v));
		EXPECT_EQ(v_move.x(), 1);
		EXPECT_EQ(v_move.y(), 2);
		EXPECT_EQ(v_move.z(), 3);
	}
	{ // += -= *= /= operator
		Vector3<float> v1(1, 2, 3);
		Vector3<float> v2(2, 3, 4);

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
		Vector3<float> v1(1, 2, 3);
		Vector3<float> v2(2, 3, 4);
		auto res = v1.dot(v2);
		EXPECT_EQ(res, 20);
	}
	{ // cross
		Vector3<float> v1(1, 2, 3);
		Vector3<float> v2(2, 3, 4);
		auto res = v1.cross(v2);
		EXPECT_EQ(res.x(), -1);
		EXPECT_EQ(res.y(), 2);
		EXPECT_EQ(res.z(), -1);
	}
}

TEST(TestMatrix, Matrix3x3f)
{
//	EXPECT_EQ(sizeof(Matrix3x3<char>), 9 * sizeof(char));
//	EXPECT_EQ(sizeof(Matrix3x3<short>), 9 * sizeof(short));
//	EXPECT_EQ(sizeof(Matrix3x3<int>), 9 * sizeof(int));
//	EXPECT_EQ(sizeof(Matrix3x3<float>), 9 * sizeof(float));
//	EXPECT_EQ(sizeof(Matrix3x3<double>), 9 * sizeof(double));
//
//	{
//		EXPECT_EQ(Matrix3x3<float>::Zero()._m, Eigen::Matrix3f::Zero());
//		EXPECT_EQ(Matrix3x3<float>::Identity()._m, Eigen::Matrix3f::Identity());
//		EXPECT_EQ(Matrix3x3<float>()._m, Eigen::Matrix3f::Zero());
//	}
//	{
//		Matrix3x3<float> m1 = Matrix3x3<float>({1, 2, 3, 3, 2, 1, 2, 1, 3}).inverse();
//		Eigen::Matrix3f inv;
//		inv << -5, 3, 4, 7, 3, -8, 1, -3, 4;
//		inv /= 12.f;
//		Matrix3x3<float> m2(inv);
//		EXPECT_TRUE(similar(m1, m2));
//	}
}

#endif //HINAPE_TEST_MATH_H
