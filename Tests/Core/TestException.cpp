#include <gtest/gtest.h>

#include <string_view>

#include <Aonir/Core/Utils/Exception.hpp>

using namespace Aonir;

namespace
{
    class TestException : public BaseException<int>
    {
    public:
        using BaseException::BaseException;
    };
}

TEST(Exception, Usage) // NOLINT
{
    auto e = TestException("DefaultData");

    ASSERT_EQ(std::string_view(e.what()), "DefaultData");

    ASSERT_EQ(e.GetMessage(), "DefaultData");
    ASSERT_EQ(e.GetSource().line(), 20);
    ASSERT_GT(e.GetStacktrace().size(), 0);
    ASSERT_EQ(e.GetData(), 0);

    e = TestException("CustomData", 3);

    ASSERT_EQ(e.GetMessage(), "CustomData");
    ASSERT_EQ(e.GetData(), 3);
}
