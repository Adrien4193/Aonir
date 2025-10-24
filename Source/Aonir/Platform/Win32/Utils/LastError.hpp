#pragma once

#include <string>
#include <string_view>

#include <Aonir/Core/Utils/Exception.hpp>

namespace Aonir
{
    class Win32Exception : public BaseException<NoExceptionData>
    {
    public:
        using BaseException::BaseException;
    };

    auto FormatWin32LastError(std::string_view message) -> std::string;
    auto Win32LastError(std::string_view message) -> Win32Exception;
}
