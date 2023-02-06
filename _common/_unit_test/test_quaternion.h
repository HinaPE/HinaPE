#ifndef HINAPE_TEST_QUATERNION_H
#define HINAPE_TEST_QUATERNION_H
#include "gtest/gtest.h"

#include "math/quaternion.h"

#include <iostream>

TEST(TestQuaternion, Quaternion)
{
	using namespace HinaPE::Math;

	// valid check quaternion
	Quaternion<real> q;
	q._q.w() = 1;
	q._q.x() = 2;
	q._q.y() = 3;
	q._q.z() = 4;

	// constructors
	Quaternion<real> q0;
	Quaternion<real> q1 = Quaternion<real>::Identity();
	Quaternion<real> q2(1, 2, 3, 4);
	Quaternion<real> q3{1, 2, 3, 4};
	Quaternion<real> q4({1, 2, 3, 4});
	Quaternion<real> q5 = {1, 2, 3, 4};
	Quaternion<real> q6 = Quaternion<real>(1, 2, 3, 4);

	EXPECT_EQ(Quaternion<real>::Identity(), q0);
	EXPECT_EQ(Quaternion<real>::Identity(), q1);
	EXPECT_EQ(q, q2);
	EXPECT_EQ(q, q3);
	EXPECT_EQ(q, q4);
	EXPECT_EQ(q, q5);
	EXPECT_EQ(q, q6);

	//
}

#endif //HINAPE_TEST_QUATERNION_H
