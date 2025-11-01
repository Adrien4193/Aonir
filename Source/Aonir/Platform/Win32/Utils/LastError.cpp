#include "LastError.hpp"

#include <format>
#include <string>
#include <string_view>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <errhandlingapi.h>

namespace Aonir
{
    auto FormatWin32LastError(std::string_view message) -> std::string
    {
        auto code = GetLastError();
        return std::format("{} (last error = {})", message, code);
    }

    auto Win32LastError(std::string_view message) -> Win32Exception
    {
        auto description = FormatWin32LastError(message);
        return Win32Exception{description};
    }
}
