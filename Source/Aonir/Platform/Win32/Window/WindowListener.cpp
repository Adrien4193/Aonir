#include "WindowListener.hpp"

#include <cstddef>
#include <utility>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>
#include <minwindef.h>
#include <windef.h>

#include <Aonir/Core/Window/WindowEvents.hpp>
#include <Aonir/Platform/Win32/Utils/StringConversion.hpp>

namespace
{
    using namespace Aonir;

    auto OnSetTitle(const WindowListener &listener, LPARAM lparam) -> void
    {
        const auto *wtitle = reinterpret_cast<const wchar_t *>(lparam); // NOLINT(performance-no-int-to-ptr)
        auto title = ToUtf8(wtitle);

        listener(WindowRename(std::move(title)));
    }

    auto OnMove(const WindowListener &listener, LPARAM lparam) -> void
    {
        auto x = static_cast<std::size_t>(LOWORD(lparam));
        auto y = static_cast<std::size_t>(HIWORD(lparam));

        listener(WindowMove({x, y}));
    }

    auto OnResize(const WindowListener &listener, LPARAM lparam) -> void
    {
        auto width = static_cast<std::size_t>(LOWORD(lparam));
        auto height = static_cast<std::size_t>(HIWORD(lparam));

        listener(WindowResize({width, height}));
    }

    auto OnClose(const WindowListener &listener) -> void
    {
        listener(WindowClose());
    }
}

namespace Aonir
{
    Win32WindowListener::Win32WindowListener(WindowListener listener):
        m_listener(std::move(listener))
    {
    }

    auto Win32WindowListener::ProcessEvent(HWND window, UINT type, WPARAM wparam, LPARAM lparam) -> LRESULT
    {
        switch (type)
        {
        case WM_SETTEXT:
            OnSetTitle(m_listener, lparam);
            break;
        case WM_MOVE:
            OnMove(m_listener, lparam);
            return 0;
        case WM_SIZE:
            OnResize(m_listener, lparam);
            return 0;
        case WM_CLOSE:
            OnClose(m_listener);
            return 0;
        default:
            break;
        }

        return DefWindowProcW(window, type, wparam, lparam);
    }
}
