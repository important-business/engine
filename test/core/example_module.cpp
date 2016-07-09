#include "gtest/gtest.h"
#include "core/example_module.hpp"

using namespace core;

TEST(module_get_set_test, zero_test)
{
    auto a = core::ExampleModule(0);
    ASSERT_EQ(0, a.getme());
    a.setme(0);
    ASSERT_EQ(0, a.getme());
}

TEST(module_get_set_test, positive_test)
{
    auto a = core::ExampleModule(4);
    ASSERT_EQ(4, a.getme());
    a.setme(15);
    ASSERT_EQ(15, a.getme());
}

/* This test fails on purpose! */
TEST(module_get_set_test, negative_test)
{
    auto a = core::ExampleModule(-2);
    ASSERT_EQ(-2, a.getme());
    a.setme(-5);
    ASSERT_EQ(-5, a.getme());
}
