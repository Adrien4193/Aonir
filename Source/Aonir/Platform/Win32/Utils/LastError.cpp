#include "LastError.hpp"

#include <format>
#include <string>
#include <string_view>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <errhandlingapi.h>

namespace Aonir
{
    auto FormatLastError(std::string_view message) -> std::string
    {
        auto code = GetLastError();
        return std::format("{} (last error = {})", message, code);
    }

    auto LastError(std::string_view message) -> SystemException
    {
        auto description = FormatLastError(message);
        return SystemException{description};
    }
}
