#include "WindowListener.hpp"

#include <cstdint>
#include <exception>
#include <utility>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>
#include <minwindef.h>
#include <windef.h>
#include <windowsx.h>

#include <Aonir/Core/Window/Events.hpp>
#include <Aonir/Core/Window/Mouse.hpp>
#include <Aonir/Platform/Win32/Utils/StringConversion.hpp>

namespace
{
    using namespace Aonir;

    auto DeserializeMouseMove(LPARAM lparam) -> MouseMove
    {
        const auto x = GET_X_LPARAM(lparam);
        const auto y = GET_Y_LPARAM(lparam);
        return {x, y};
    }

    auto DeserializeMouseScroll(WPARAM wparam, bool vertical) -> MouseScroll
    {
        const auto delta = GET_WHEEL_DELTA_WPARAM(wparam);

        if (vertical)
        {
            return {0, delta};
        }

        return {delta, 0};
    }

    auto DeserializeWindowRename(LPARAM lparam) -> WindowRename
    {
        const auto *wtitle = reinterpret_cast<const wchar_t *>(lparam); // NOLINT(performance-no-int-to-ptr)
        return {ToUtf8(wtitle)};
    }

    auto DeserializeWindowMove(LPARAM lparam) -> WindowMove
    {
        const auto x = GET_X_LPARAM(lparam);
        const auto y = GET_Y_LPARAM(lparam);
        return {x, y};
    }

    auto DeserializeWindowResize(LPARAM lparam) -> WindowResize
    {
        auto width = static_cast<std::uint32_t>(LOWORD(lparam));
        auto height = static_cast<std::uint32_t>(HIWORD(lparam));
        return {width, height};
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
            switch (type)
            {
            case WM_MOUSEMOVE:
                m_listener(DeserializeMouseMove(lparam));
                return 0;
            case WM_LBUTTONDOWN:
                m_listener(MouseButtonPress(MouseButton::Left));
                return 0;
            case WM_LBUTTONUP:
                m_listener(MouseButtonRelease(MouseButton::Left));
                return 0;
            case WM_RBUTTONDOWN:
                m_listener(MouseButtonPress(MouseButton::Right));
                return 0;
            case WM_RBUTTONUP:
                m_listener(MouseButtonRelease(MouseButton::Right));
                return 0;
            case WM_MBUTTONDOWN:
                m_listener(MouseButtonPress(MouseButton::Middle));
                return 0;
            case WM_MBUTTONUP:
                m_listener(MouseButtonRelease(MouseButton::Middle));
                return 0;
            case WM_MOUSEWHEEL:
                m_listener(DeserializeMouseScroll(wparam, true));
                return 0;
            case WM_MOUSEHWHEEL:
                m_listener(DeserializeMouseScroll(wparam, false));
                return 0;
            case WM_SETTEXT:
                m_listener(DeserializeWindowRename(lparam));
                break;
            case WM_MOVE:
                m_listener(DeserializeWindowMove(lparam));
                return 0;
            case WM_SIZE:
                m_listener(DeserializeWindowResize(lparam));
                return 0;
            case WM_CLOSE:
                m_listener(WindowClose());
                return 0;
            default:
                break;
            }
        }
        catch (...)
        {
            m_listener(WindowError(std::current_exception()));
        }

        return DefWindowProcW(window, type, wparam, lparam);
    }
}
