#include "gtest/gtest.h"
#include "core/math.hpp"

using namespace core;

TEST(math_vector_add, add_zero_test)
{
    auto a = core::Vector(5.0f, 5.0f);
    auto b = core::Vector(0.0f, 0.0f);
    auto result = a + b;
    ASSERT_FLOAT_EQ(5.0f, result.x);
    ASSERT_FLOAT_EQ(5.0f, result.y);
    a += b;
    ASSERT_FLOAT_EQ(5.0f, a.x);
    ASSERT_FLOAT_EQ(5.0f, a.y);
}

TEST(math_vector_add, add_positive_test)
{
    auto a = core::Vector(40.0f, 50.0f);
    auto b = core::Vector(20.0f, 42.0f);
    auto result = a + b;
    ASSERT_FLOAT_EQ(60.0f, result.x);
    ASSERT_FLOAT_EQ(92.0f, result.y);
    a += b;
    ASSERT_FLOAT_EQ(60.0f, a.x);
    ASSERT_FLOAT_EQ(92.0f, a.y);
}

TEST(math_vector_add, add_negative_test)
{
    auto a = core::Vector(-400.0f, -24.0f);
    auto b = core::Vector(-90.0f, -201.0f);
    auto result = a + b;
    ASSERT_FLOAT_EQ(-490.0f, result.x);
    ASSERT_FLOAT_EQ(-225.0f, result.y);
    a += b;
    ASSERT_FLOAT_EQ(-490.0f, a.x);
    ASSERT_FLOAT_EQ(-225.0f, a.y);
}

TEST(math_vector_mult, mult_zero_test)
{
    auto a = core::Vector(5.0f, 5.0f);
    auto b = core::Vector(0.0f, 0.0f);
    auto result = a * b;
    ASSERT_FLOAT_EQ(0.0f, result.x);
    ASSERT_FLOAT_EQ(0.0f, result.y);
    a *= b;
    ASSERT_FLOAT_EQ(0.0f, a.x);
    ASSERT_FLOAT_EQ(0.0f, a.y);
}

TEST(math_vector_mult, mult_positive_test)
{
    auto a = core::Vector(40.0f, 50.0f);
    auto b = core::Vector(20.0f, 42.0f);
    auto result = a * b;
    ASSERT_FLOAT_EQ(800.0f, result.x);
    ASSERT_FLOAT_EQ(2100.0f, result.y);
    a *= b;
    ASSERT_FLOAT_EQ(800.0f, a.x);
    ASSERT_FLOAT_EQ(2100.0f, a.y);
}

TEST(math_vector_mult, mult_negative_test)
{
    auto a = core::Vector(-400.0f, -24.0f);
    auto b = core::Vector(-90.0f, -201.0f);
    auto result = a * b;
    ASSERT_FLOAT_EQ(36000.0f, result.x);
    ASSERT_FLOAT_EQ(4824.0f, result.y);
    a *= b;
    ASSERT_FLOAT_EQ(36000.0f, a.x);
    ASSERT_FLOAT_EQ(4824.0f, a.y);
}

TEST(math_vector_sub, sub_zero_test)
{
    auto a = core::Vector(5.0f, 5.0f);
    auto b = core::Vector(0.0f, 0.0f);
    auto result = a - b;
    ASSERT_FLOAT_EQ(5.0f, result.x);
    ASSERT_FLOAT_EQ(5.0f, result.y);
    a -= b;
    ASSERT_FLOAT_EQ(5.0f, a.x);
    ASSERT_FLOAT_EQ(5.0f, a.y);
}

TEST(math_vector_sub, sub_positive_test)
{
    auto a = core::Vector(40.0f, 50.0f);
    auto b = core::Vector(20.0f, 42.0f);
    auto result = a - b;
    ASSERT_FLOAT_EQ(20.0f, result.x);
    ASSERT_FLOAT_EQ(8.0f, result.y);
    a -= b;
    ASSERT_FLOAT_EQ(20.0f, a.x);
    ASSERT_FLOAT_EQ(8.0f, a.y);
}

TEST(math_vector_sub, sub_negative_test)
{
    auto a = core::Vector(-400.0f, -24.0f);
    auto b = core::Vector(-90.0f, -201.0f);
    auto result = a - b;
    ASSERT_FLOAT_EQ(-310.0f, result.x);
    ASSERT_FLOAT_EQ(177.0f, result.y);
    a -= b;
    ASSERT_FLOAT_EQ(-310.0f, a.x);
    ASSERT_FLOAT_EQ(177.0f, a.y);
}

TEST(math_vector_dot, dot_zero_test)
{
    auto a = core::Vector(5.0f, 5.0f);
    auto b = core::Vector(0.0f, 0.0f);
    auto result = a.dot(b);
    ASSERT_FLOAT_EQ(0.0f, result);
}

TEST(math_vector_dot, dot_positive_test)
{
    auto a = core::Vector(40.0f, 50.0f);
    auto b = core::Vector(20.0f, 42.0f);
    auto result = a.dot(b);
    ASSERT_FLOAT_EQ(2900.0f, result);
}

TEST(math_vector_dot, dot_negative_test)
{
    auto a = core::Vector(-400.0f, -24.0f);
    auto b = core::Vector(-90.0f, -201.0f);
    auto result = a.dot(b);
    ASSERT_FLOAT_EQ(40824.0f, result);
}

TEST(math_vector_mag, mag_zero_test)
{
    auto a = core::Vector(0.0f, 0.0f);
    auto result = a.mag();
    ASSERT_FLOAT_EQ(0.0f, result);
}

TEST(math_vector_mag, mag_positive_test)
{
    auto a = core::Vector(8.0f, 15.0f);
    auto result = a.mag();
    ASSERT_FLOAT_EQ(17.0f, result);
}

TEST(math_vector_mag, mag_negative_test)
{
    auto a = core::Vector(-7.0f, -24.0f);
    auto result = a.mag();
    ASSERT_FLOAT_EQ(25.0f, result);
}

TEST(math_vector_normalize, normalize_zero_test)
{
    auto a = core::Vector(0.0f, 0.0f);
    auto result = a.normalize();
    ASSERT_FLOAT_EQ(0.0f, result.mag());
    ASSERT_FLOAT_EQ(0.0f, result.x);
    ASSERT_FLOAT_EQ(0.0f, result.y);
}

TEST(math_vector_normalize, normalize_positive_test)
{
    auto a = core::Vector(8.0f, 15.0f);
    auto result = a.normalize();
    ASSERT_FLOAT_EQ(1.0f, result.mag());
    ASSERT_FLOAT_EQ(8.0f / 17.f, result.x);
    ASSERT_FLOAT_EQ(15.0f / 17.f, result.y);
}

TEST(math_vector_normalize, normalize_negative_test)
{
    auto a = core::Vector(-7.0f, -24.0f);
    auto result = a.normalize();
    ASSERT_FLOAT_EQ(1.0f, result.mag());
    ASSERT_FLOAT_EQ(-7.0f / 25.f, result.x);
    ASSERT_FLOAT_EQ(-24.0f / 25.f, result.y);
}
