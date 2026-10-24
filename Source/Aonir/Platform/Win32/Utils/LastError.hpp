#pragma once

#include <string>
#include <string_view>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <minwindef.h>

#include <Aonir/Core/Utils/Exception.hpp>

namespace Aonir
{
    class Win32Error : public Exception
    {
    private:
        DWORD m_code;

    public:
        explicit Win32Error(std::string message, DWORD code);

        [[nodiscard]] auto GetCode() const -> DWORD;
    };

    auto Win32ErrorToString(DWORD code) -> std::string;
    auto Win32LastError(std::string_view message) -> Win32Error;
}
