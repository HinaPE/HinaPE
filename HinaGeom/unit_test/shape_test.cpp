#include <gtest/gtest.h>

#include "simple_shapes/sphere.h"

#include <vector>
#include <iostream>

#include <type_traits>

enum Foo
{
    A = 3,
    B = 4
};

template<Foo T>
class Bar
{
public:

    template<class A>
    static void foo(A *)
    {_foo();}

    static void _foo();
};

template<Foo T>
void Bar<T>::_foo()
{
    return;
}

// Demonstrate some basic assertions.
TEST(ShapeTest, BasicAssertions)
{
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(7 * 6, 42);

    Bar<B> b;

    int a = 1;
    b.foo<int>(&a);
}
