#include "common/logging.hpp"
#include <iostream>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    core::logging_init();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
