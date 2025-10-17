#include "Exception.hpp"

#include <source_location>
#include <stacktrace>
#include <string>
#include <utility>

namespace Aonir
{
    Exception::Exception(std::string message, const std::source_location &source, std::stacktrace stacktrace):
        m_message(std::move(message)),
        m_source(source),
        m_stacktrace(std::move(stacktrace))
    {
    }

    auto Exception::what() const -> const char *
    {
        return m_message.c_str();
    }

    auto Exception::GetMessage() const -> const std::string &
    {
        return m_message;
    }

    auto Exception::GetSource() const -> const std::source_location &
    {
        return m_source;
    }

    auto Exception::GetStacktrace() const -> const std::stacktrace &
    {
        return m_stacktrace;
    }
}
