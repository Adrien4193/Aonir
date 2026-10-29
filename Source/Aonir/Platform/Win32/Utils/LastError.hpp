#pragma once

#include <string>
#include <string_view>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <minwindef.h>

#include <Aonir/Core/Utils/Exception.hpp>

namespace Aonir
{
    class Win32Exception : public Exception
    {
    private:
        DWORD m_code;

    public:
        AONIR_PLATFORM_API explicit Win32Exception(std::string message, DWORD code);

        [[nodiscard]] auto GetCode() const -> DWORD;
    };

    AONIR_PLATFORM_API auto GetWin32ErrorMessage(DWORD code) -> std::string;
    AONIR_PLATFORM_API auto Win32LastError(std::string_view message) -> Win32Exception;
}
