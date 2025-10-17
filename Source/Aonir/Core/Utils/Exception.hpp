#pragma once

#include <concepts>
#include <exception>
#include <source_location>
#include <stacktrace>
#include <string>

namespace Aonir
{
    class Exception : std::exception
    {
    private:
        std::string m_message;
        std::source_location m_source;
        std::stacktrace m_stacktrace;

    public:
        AONIR_CORE_API explicit Exception(std::string message, const std::source_location &source, std::stacktrace stacktrace);

        AONIR_CORE_API auto what() const -> const char * override;

        AONIR_CORE_API auto GetMessage() const -> const std::string &;
        AONIR_CORE_API auto GetSource() const -> const std::source_location &;
        AONIR_CORE_API auto GetStacktrace() const -> const std::stacktrace &;
    };

    struct NoExceptionData
    {
    };

    template<typename T>
    class BaseException : public Exception
    {
    private:
        T m_data{};

    public:
        explicit BaseException(
            std::string message,
            const std::source_location &source = std::source_location::current(),
            std::stacktrace stacktrace = std::stacktrace::current()) requires std::constructible_from<T>:
            Exception(std::move(message), source, std::move(stacktrace))
        {
        }

        explicit BaseException(
            std::string message,
            T data,
            const std::source_location &source = std::source_location::current(),
            std::stacktrace stacktrace = std::stacktrace::current()):
            Exception(std::move(message), source, std::move(stacktrace)),
            m_data(std::move(data))
        {
        }

        auto GetData() const -> const T &
        {
            return m_data;
        }
    };
}
