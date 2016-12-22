#include "gtest/gtest.h"
#include "core/math.hpp"

using namespace core;

TEST(math_vector_add, add_zero_test)
{
    auto a = core::Vector(5, 5);
    auto b = core::Vector(0, 0);
    auto result = a + b;
    ASSERT_EQ(5, result.x);
    ASSERT_EQ(5, result.y);
}

TEST(math_vector_add, add_positive_test)
{
    auto a = core::Vector(40, 50);
    auto b = core::Vector(20, 42);
    auto result = a + b;
    ASSERT_EQ(60, result.x);
    ASSERT_EQ(92, result.y);
}

TEST(math_vector_add, add_negative_test)
{
    auto a = core::Vector(-400, -24);
    auto b = core::Vector(-90, -201);
    auto result = a + b;
    ASSERT_EQ(-490, result.x);
    ASSERT_EQ(-225, result.y);
}

TEST(math_vector_mult, mult_zero_test)
{
    auto a = core::Vector(5, 5);
    auto b = core::Vector(0, 0);
    auto result = a * b;
    ASSERT_EQ(0, result.x);
    ASSERT_EQ(0, result.y);
}

TEST(math_vector_mult, mult_positive_test)
{
    auto a = core::Vector(40, 50);
    auto b = core::Vector(20, 42);
    auto result = a * b;
    ASSERT_EQ(800, result.x);
    ASSERT_EQ(2100, result.y);
}

TEST(math_vector_mult, mult_negative_test)
{
    auto a = core::Vector(-400, -24);
    auto b = core::Vector(-90, -201);
    auto result = a * b;
    ASSERT_EQ(36000, result.x);
    ASSERT_EQ(4824, result.y);
}

TEST(math_vector_sub, sub_zero_test)
{
    auto a = core::Vector(5, 5);
    auto b = core::Vector(0, 0);
    auto result = a - b;
    ASSERT_EQ(5, result.x);
    ASSERT_EQ(5, result.y);
}

TEST(math_vector_sub, sub_positive_test)
{
    auto a = core::Vector(40, 50);
    auto b = core::Vector(20, 42);
    auto result = a - b;
    ASSERT_EQ(20, result.x);
    ASSERT_EQ(8, result.y);
}

TEST(math_vector_sub, sub_negative_test)
{
    auto a = core::Vector(-400, -24);
    auto b = core::Vector(-90, -201);
    auto result = a - b;
    ASSERT_EQ(-310, result.x);
    ASSERT_EQ(177, result.y);
}

TEST(math_vector_dot, dot_zero_test)
{
    auto a = core::Vector(5, 5);
    auto b = core::Vector(0, 0);
    auto result = a.dot(b);
    ASSERT_EQ(0, result);
}

TEST(math_vector_dot, dot_positive_test)
{
    auto a = core::Vector(40, 50);
    auto b = core::Vector(20, 42);
    auto result = a.dot(b);
    ASSERT_EQ(2900, result);
}

TEST(math_vector_dot, dot_negative_test)
{
    auto a = core::Vector(-400, -24);
    auto b = core::Vector(-90, -201);
    auto result = a.dot(b);
    ASSERT_EQ(40824, result);
}
