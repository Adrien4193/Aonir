#include "LastError.hpp"

#include <array>
#include <cstdarg>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>
#include <utility>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinBase.h>
#include <errhandlingapi.h>
#include <minwindef.h>
#include <winnt.h>

#include <Aonir/Core/Utils/Exception.hpp>

#include "StringConversion.hpp"

namespace Aonir
{
    Win32Error::Win32Error(std::string message, DWORD code):
        Exception(std::move(message)),
        m_code(code)
    {
    }

    auto Win32Error::GetCode() const -> DWORD
    {
        return m_code;
    }

    auto Win32ErrorToString(DWORD code) -> std::string
    {
        auto buffer = std::array<wchar_t, 256>();

        auto flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        const auto *source = static_cast<LPCVOID>(nullptr);
        auto language = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
        auto *data = buffer.data();
        auto size = static_cast<DWORD>(buffer.size());
        auto *args = static_cast<va_list *>(nullptr);

        auto result = FormatMessageW(flags, source, code, language, data, size, args);

        if (result == 0)
        {
            return "Unknown error";
        }

        auto length = static_cast<std::size_t>(result);

        return ToUtf8({data, length});
    }

    auto Win32LastError(std::string_view message) -> Win32Error
    {
        auto code = GetLastError();
        auto description = Win32ErrorToString(code);
        auto extended = std::format("{}: {}", message, description);
        return Win32Error(std::move(extended), code);
    }
}
