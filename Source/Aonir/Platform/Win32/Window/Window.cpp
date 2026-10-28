#include "Window.hpp"

#include <cassert>
#include <memory>
#include <string_view>
#include <utility>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>
#include <minwindef.h>
#include <windef.h>

#include "Aonir/Core/Window/WindowEvent.hpp"

#include <Aonir/Platform/Win32/Utils/LastError.hpp>
#include <Aonir/Platform/Win32/Utils/StringConversion.hpp>

namespace Aonir
{
    auto Win32Window::Deleter::operator()(HWND handle) const -> void
    {
        [[maybe_unused]] const auto success = DestroyWindow(handle);
        assert(success == TRUE);
    }

    Win32Window::Win32Window(HWND handle, std::unique_ptr<WindowListener> listener):
        m_handle(handle),
        m_listener(std::move(listener))
    {
    }

    auto Win32Window::GetNativeWindow() const -> HWND
    {
        return m_handle.get();
    }

    auto Win32Window::Show() -> void
    {
        ShowWindow(m_handle.get(), SW_NORMAL);
    }

    auto Win32Window::SetTitle(std::string_view title) -> void
    {
        const auto wtitle = ToUtf16(title);

        const auto success = SetWindowTextW(m_handle.get(), wtitle.c_str());

        if (success == FALSE)
        {
            throw Win32LastError("Cannot set window title");
        }
    }

    auto Win32Window::SetPosition(const WindowPosition &position) -> void
    {
        const auto flags = static_cast<UINT>(SWP_NOSIZE | SWP_NOACTIVATE);

        const auto x = static_cast<int>(position.x);
        const auto y = static_cast<int>(position.y);

        const auto success = SetWindowPos(m_handle.get(), nullptr, x, y, 0, 0, flags);

        if (success == FALSE)
        {
            throw Win32LastError("Cannot set window position");
        }
    }

    auto Win32Window::Resize(const WindowSize &size) -> void
    {
        const auto flags = static_cast<UINT>(SWP_NOMOVE | SWP_NOACTIVATE);

        const auto width = static_cast<int>(size.width);
        const auto height = static_cast<int>(size.height);

        const auto success = SetWindowPos(m_handle.get(), nullptr, 0, 0, width, height, flags);

        if (success == FALSE)
        {
            throw Win32LastError("Cannot set window size");
        }
    }
}
