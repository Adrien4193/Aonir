#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <string>
#include <string_view>

namespace Aonir
{
    enum class LogLevel : std::uint8_t
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        Off,
    };

    AONIR_CORE_API auto GetLogLevelName(LogLevel level) -> std::string_view;

    struct LogRecord
    {
        std::string_view name;
        LogLevel level = LogLevel::Info;
        std::string_view message;
    };

    AONIR_CORE_API auto FormatLogRecord(const LogRecord &record) -> std::string;

    using LogHandler = std::function<auto(const LogRecord &)->void>;

    class Logger
    {
    private:
        std::string m_name;
        LogLevel m_level;
        LogHandler m_handler;

    public:
        AONIR_CORE_API explicit Logger(std::string name, LogLevel level, LogHandler handler);

        AONIR_CORE_API auto GetName() const -> const std::string &;
        AONIR_CORE_API auto GetLevel() const -> LogLevel;
        AONIR_CORE_API auto SetLevel(LogLevel level) -> void;
        AONIR_CORE_API auto IsEnabled(LogLevel level) const -> bool;

        template<typename... Args>
        auto Log(LogLevel level, std::format_string<Args...> format, Args &&...args) -> void
        {
            if (!IsEnabled(level))
            {
                return;
            }

            auto message = std::format(format, std::forward<Args>(args)...);
            auto record = LogRecord(m_name, m_level, message);

            m_handler(record);
        }

        template<typename... Args>
        auto Trace(std::format_string<Args...> format, Args &&...args) -> void
        {
            Log(LogLevel::Trace, format, std::forward<Args>(args)...);
        }

        template<typename... Args>
        auto Debug(std::format_string<Args...> format, Args &&...args) -> void
        {
            Log(LogLevel::Debug, format, std::forward<Args>(args)...);
        }

        template<typename... Args>
        auto Info(std::format_string<Args...> format, Args &&...args) -> void
        {
            Log(LogLevel::Info, format, std::forward<Args>(args)...);
        }

        template<typename... Args>
        auto Warn(std::format_string<Args...> format, Args &&...args) -> void
        {
            Log(LogLevel::Warn, format, std::forward<Args>(args)...);
        }

        template<typename... Args>
        auto Error(std::format_string<Args...> format, Args &&...args) -> void
        {
            Log(LogLevel::Error, format, std::forward<Args>(args)...);
        }

        template<typename... Args>
        auto Fatal(std::format_string<Args...> format, Args &&...args) -> void
        {
            Log(LogLevel::Fatal, format, std::forward<Args>(args)...);
        }
    };
}
