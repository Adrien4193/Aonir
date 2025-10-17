#include <gtest/gtest.h>

#include <string_view>

#include <Aonir/Core/Utils/Exception.hpp>

using namespace Aonir;

class TestException : public BaseException<int>
{
public:
    using BaseException::BaseException;
};

TEST(Exception, Usage)
{
    auto e = TestException("Test1");

    ASSERT_EQ(std::string_view(e.what()), "Test1");

    ASSERT_EQ(e.GetMessage(), "Test1");
    ASSERT_EQ(e.GetSource().line(), 17);
    ASSERT_GT(e.GetStacktrace().size(), 0);
    ASSERT_EQ(e.GetData(), 0);

    e = TestException("Test2", 3);

    ASSERT_EQ(e.GetMessage(), "Test2");
    ASSERT_EQ(e.GetData(), 3);
}
