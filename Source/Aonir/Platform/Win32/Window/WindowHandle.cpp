#include "WindowHandle.hpp"

#include <cassert>
#include <string_view>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>
#include <minwindef.h>
#include <windef.h>

#include "Aonir/Core/Window/WindowEvents.hpp"

#include <Aonir/Platform/Win32/Utils/LastError.hpp>
#include <Aonir/Platform/Win32/Utils/StringConversion.hpp>

namespace Aonir
{
    auto Win32WindowHandle::Deleter::operator()(HWND handle) const -> void
    {
        [[maybe_unused]] auto success = DestroyWindow(handle);
        assert(success == TRUE);
    }

    Win32WindowHandle::Win32WindowHandle(HWND handle):
        m_handle(handle)
    {
    }

    auto Win32WindowHandle::GetNativeHandle() const -> HWND
    {
        return m_handle.get();
    }

    auto Win32WindowHandle::Show(int code) -> void
    {
        ShowWindow(m_handle.get(), code);
    }

    auto Win32WindowHandle::SetTitle(std::string_view title) -> void
    {
        auto wtitle = ToUtf16(title);

        auto success = SetWindowTextW(m_handle.get(), wtitle.c_str());

        if (success == FALSE)
        {
            throw Win32LastError("Failed to set window title");
        }
    }

    auto Win32WindowHandle::SetPosition(const WindowPosition &position) -> void
    {
        auto flags = UINT(SWP_NOSIZE | SWP_NOACTIVATE);

        auto x = static_cast<int>(position.x);
        auto y = static_cast<int>(position.y);

        auto success = SetWindowPos(m_handle.get(), nullptr, x, y, 0, 0, flags);

        if (success == FALSE)
        {
            throw Win32LastError("Failed to set window position");
        }
    }

    auto Win32WindowHandle::Resize(const WindowSize &size) -> void
    {
        auto flags = UINT(SWP_NOMOVE | SWP_NOACTIVATE);

        auto width = static_cast<int>(size.width);
        auto height = static_cast<int>(size.height);

        auto success = SetWindowPos(m_handle.get(), nullptr, 0, 0, width, height, flags);

        if (success == FALSE)
        {
            throw Win32LastError("Failed to set window size");
        }
    }
}
