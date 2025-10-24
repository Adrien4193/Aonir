#pragma once

#include <string>
#include <string_view>

#include <Aonir/Core/Utils/Exception.hpp>

namespace Aonir
{
    class SystemException : public BaseException<NoExceptionData>
    {
    public:
        using BaseException::BaseException;
    };

    auto FormatLastError(std::string_view message) -> std::string;
    auto LastError(std::string_view message) -> SystemException;
}
