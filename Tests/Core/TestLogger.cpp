#include <gtest/gtest.h>

#include <Aonir/Core/Utils/Logger.hpp>

using namespace Aonir;

TEST(Logger, LevelName) // NOLINT
{
    ASSERT_EQ(GetLogLevelName(LogLevel::Trace), "Trace");
    ASSERT_EQ(GetLogLevelName(LogLevel::Debug), "Debug");
    ASSERT_EQ(GetLogLevelName(LogLevel::Info), "Info");
    ASSERT_EQ(GetLogLevelName(LogLevel::Warn), "Warn");
    ASSERT_EQ(GetLogLevelName(LogLevel::Error), "Error");
    ASSERT_EQ(GetLogLevelName(LogLevel::Fatal), "Fatal");
}

TEST(Logger, RecordFormat) // NOLINT
{
    auto message = FormatLogRecord({
        .name = "Test",
        .level = LogLevel::Debug,
        .message = "This is a test.",
    });

    ASSERT_EQ(message, "[Debug][Test]: This is a test.");
}

TEST(Logger, Usage) // NOLINT
{
    auto called = false;
    auto logger = Logger("Test", LogLevel::Debug, [&](const auto &) { called = true; });

    ASSERT_EQ(logger.GetName(), "Test");

    ASSERT_EQ(logger.GetLevel(), LogLevel::Debug);

    ASSERT_TRUE(logger.IsEnabled(LogLevel::Debug));
    ASSERT_FALSE(logger.IsEnabled(LogLevel::Trace));
    ASSERT_TRUE(logger.IsEnabled(LogLevel::Info));

    logger.Trace("Not logged");
    ASSERT_FALSE(called);

    logger.Info("Logged");
    ASSERT_TRUE(called);
}
