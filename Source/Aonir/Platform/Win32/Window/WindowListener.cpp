#include "WindowListener.hpp"

#include <cstdint>
#include <exception>
#include <optional>
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

    struct Win32Message
    {
        UINT type;
        WPARAM wparam;
        LPARAM lparam;
    };

    auto DeserializeWindowRename(LPARAM lparam) -> WindowRename
    {
        const auto *wtitle = reinterpret_cast<const wchar_t *>(lparam); // NOLINT(performance-no-int-to-ptr)
        return {ToUtf8(wtitle)};
    }

    auto DeserializeWindowMove(LPARAM lparam) -> WindowMove
    {
        auto x = static_cast<std::uint32_t>(LOWORD(lparam));
        auto y = static_cast<std::uint32_t>(HIWORD(lparam));

        return {x, y};
    }

    auto DeserializeWindowResize(LPARAM lparam) -> WindowResize
    {
        auto width = static_cast<std::uint32_t>(LOWORD(lparam));
        auto height = static_cast<std::uint32_t>(HIWORD(lparam));

        return {width, height};
    }

    auto DeserializeEvent(Win32Message message) -> std::optional<WindowEvent>
    {
        switch (message.type)
        {
        case WM_SETTEXT:
            return DeserializeWindowRename(message.lparam);
        case WM_MOVE:
            return DeserializeWindowMove(message.lparam);
        case WM_SIZE:
            return DeserializeWindowResize(message.lparam);
        case WM_CLOSE:
            return WindowClose();
        default:
            return std::nullopt;
        }
    }
}

namespace Aonir
{
    Win32WindowListener::Win32WindowListener(WindowListener listener):
        m_listener(std::move(listener))
    {
    }

    auto Win32WindowListener::ProcessMessage(HWND window, UINT type, WPARAM wparam, LPARAM lparam) -> LRESULT
    {
        try
        {
            auto event = DeserializeEvent({.type = type, .wparam = wparam, .lparam = lparam});

            if (event)
            {
                m_listener(std::move(*event));
                return 0;
            }
        }
        catch (...)
        {
            m_listener(WindowError(std::current_exception()));
            return 0;
        }

        return DefWindowProcW(window, type, wparam, lparam);
    }
}
