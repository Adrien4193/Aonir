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
    Win32Exception::Win32Exception(std::string message, DWORD code):
        Exception(std::move(message)),
        m_code(code)
    {
    }

    auto Win32Exception::GetCode() const -> DWORD
    {
        return m_code;
    }

    auto GetWin32ErrorMessage(DWORD code) -> std::string
    {
        auto buffer = std::array<wchar_t, 256>();

        const auto flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        const auto *source = static_cast<LPCVOID>(nullptr);
        const auto language = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
        auto *data = buffer.data();
        const auto size = static_cast<DWORD>(buffer.size());
        auto *args = static_cast<va_list *>(nullptr);

        const auto result = FormatMessageW(flags, source, code, language, data, size, args);

        if (result == 0)
        {
            return "Unknown error";
        }

        auto length = static_cast<std::size_t>(result);

        return ToUtf8({data, length});
    }

    auto Win32LastError(std::string_view message) -> Win32Exception
    {
        const auto code = GetLastError();
        auto description = GetWin32ErrorMessage(code);
        auto extended = std::format("{}: {}", message, description);
        return Win32Exception(std::move(extended), code);
    }
}
