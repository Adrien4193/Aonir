#include <gtest/gtest.h>

#include <Template/Lib2/Lib2.hpp>

using namespace Template;

TEST(Lib2, Dummy)
{
    EXPECT_EQ(Lib2(2), 4);
}
