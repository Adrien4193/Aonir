#include "Logger.hpp"

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace Aonir
{
    auto GetLogLevelName(LogLevel level) -> std::string_view
    {
        switch (level)
        {
        case LogLevel::Trace:
            return "Trace";
        case LogLevel::Debug:
            return "Debug";
        case LogLevel::Info:
            return "Info";
        case LogLevel::Warn:
            return "Warn";
        case LogLevel::Error:
            return "Error";
        case LogLevel::Fatal:
            return "Fatal";
        case LogLevel::Off:
            return "Off";
        default:
            throw std::invalid_argument(std::to_string(static_cast<int>(level)));
        }
    }

    auto FormatLogRecord(const LogRecord &record) -> std::string
    {
        auto level = GetLogLevelName(record.level);
        return std::format("[{}][{}]: {}", level, record.name, record.message);
    }

    Logger::Logger(std::string name, LogLevel level, LogHandler handler):
        m_name(std::move(name)),
        m_level(level),
        m_handler(std::move(handler))
    {
    }

    auto Logger::GetName() const -> const std::string &
    {
        return m_name;
    }

    auto Logger::GetLevel() const -> LogLevel
    {
        return m_level;
    }

    auto Logger::SetLevel(LogLevel level) -> void
    {
        m_level = level;
    }

    auto Logger::IsEnabled(LogLevel level) const -> bool
    {
        return level >= m_level;
    }
}
