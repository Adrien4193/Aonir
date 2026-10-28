#include "WindowListener.hpp"

#include <cstdint>
#include <exception>
#include <optional>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>
#include <minwindef.h>
#include <windef.h>
#include <windowsx.h>

#include <Aonir/Core/Utils/Unicode.hpp>
#include <Aonir/Core/Window/WindowEvent.hpp>

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

    auto DeserializeKey(WPARAM wparam, LPARAM lparam) -> Key
    {
        auto virtualKey = std::optional<VirtualKey>();

        switch (LOWORD(wparam))
        {
        case VK_LEFT:
            virtualKey = VirtualKey::Left;
            break;
        case VK_RIGHT:
            virtualKey = VirtualKey::Right;
            break;
        case VK_UP:
            virtualKey = VirtualKey::Up;
            break;
        case VK_DOWN:
            virtualKey = VirtualKey::Down;
            break;
        case VK_BACK:
            virtualKey = VirtualKey::Back;
            break;
        case VK_DELETE:
            virtualKey = VirtualKey::Delete;
            break;
        case VK_RETURN:
            virtualKey = VirtualKey::Return;
            break;
        case VK_TAB:
            virtualKey = VirtualKey::Tab;
            break;
        default:
            break;
        }

        const auto physicalKey = static_cast<PhysicalKey>(LOBYTE(HIWORD(lparam)));

        return {.virtualKey = virtualKey, .physicalKey = physicalKey};
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
    auto ProcessWindowMessage(const WindowListener &listener, HWND window, UINT type, WPARAM wparam, LPARAM lparam) -> LRESULT
    {
        try
        {
            switch (type)
            {
            case WM_MOUSEMOVE:
                listener(DeserializeMouseMove(lparam));
                return 0;
            case WM_LBUTTONDOWN:
                listener(MouseButtonPress(MouseButton::Left));
                return 0;
            case WM_LBUTTONUP:
                listener(MouseButtonRelease(MouseButton::Left));
                return 0;
            case WM_RBUTTONDOWN:
                listener(MouseButtonPress(MouseButton::Right));
                return 0;
            case WM_RBUTTONUP:
                listener(MouseButtonRelease(MouseButton::Right));
                return 0;
            case WM_MBUTTONDOWN:
                listener(MouseButtonPress(MouseButton::Middle));
                return 0;
            case WM_MBUTTONUP:
                listener(MouseButtonRelease(MouseButton::Middle));
                return 0;
            case WM_MOUSEWHEEL:
                listener(DeserializeMouseScroll(wparam, true));
                return 0;
            case WM_MOUSEHWHEEL:
                listener(DeserializeMouseScroll(wparam, false));
                return 0;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                listener(KeyPress(DeserializeKey(wparam, lparam)));
                return 0;
            case WM_KEYUP:
            case WM_SYSKEYUP:
                listener(KeyRelease(DeserializeKey(wparam, lparam)));
                return 0;
            case WM_CHAR:
                listener(KeyText(static_cast<Codepoint>(wparam)));
                return 0;
            case WM_SETTEXT:
                listener(DeserializeWindowRename(lparam));
                break;
            case WM_MOVE:
                listener(DeserializeWindowMove(lparam));
                return 0;
            case WM_SIZE:
                listener(DeserializeWindowResize(lparam));
                return 0;
            case WM_CLOSE:
                listener(WindowClose());
                return 0;
            default:
                break;
            }
        }
        catch (...)
        {
            listener(WindowError(std::current_exception()));
        }

        return DefWindowProcW(window, type, wparam, lparam);
    }
}
