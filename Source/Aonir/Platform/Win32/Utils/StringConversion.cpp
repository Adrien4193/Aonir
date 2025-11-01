#include "StringConversion.hpp"

#include <cstddef>
#include <string>
#include <string_view>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinNls.h>
#include <stringapiset.h>

#include "LastError.hpp"

namespace Aonir
{
    auto ToUtf8(std::wstring_view value) -> std::string
    {
        if (value.empty())
        {
            return {};
        }

        const auto *ptr = value.data();
        auto size = static_cast<int>(value.size());

        auto required = WideCharToMultiByte(CP_UTF8, 0, ptr, size, nullptr, 0, nullptr, nullptr);

        if (required <= 0)
        {
            throw Win32LastError("Conversion preparation to UTF-16 failed");
        }

        auto result = std::string(static_cast<std::size_t>(required), '\0');

        auto written = WideCharToMultiByte(CP_UTF8, 0, ptr, size, result.data(), required, nullptr, nullptr);

        if (written != required)
        {
            throw Win32LastError("Conversion to UTF-16 failed");
        }

        return result;
    }

    auto ToUtf16(std::string_view value) -> std::wstring
    {
        if (value.empty())
        {
            return {};
        }

        const auto *ptr = value.data();
        auto size = static_cast<int>(value.size());

        auto required = MultiByteToWideChar(CP_UTF8, 0, ptr, size, nullptr, 0);

        if (required <= 0)
        {
            throw Win32LastError("Conversion preparation to UTF-8 failed");
        }

        auto result = std::wstring(static_cast<std::size_t>(required), L'\0');

        auto written = MultiByteToWideChar(CP_UTF8, 0, ptr, size, result.data(), required);

        if (written != required)
        {
            throw Win32LastError("Conversion to UTF-8 failed");
        }

        return result;
    }
}
