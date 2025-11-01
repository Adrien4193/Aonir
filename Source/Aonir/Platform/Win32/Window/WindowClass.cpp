#include "WindowClass.hpp"

#include <cassert>
#include <memory>
#include <utility>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>
#include <basetsd.h>
#include <minwindef.h>
#include <windef.h>
#include <winnt.h>

#include <Aonir/Core/Window/Window.hpp>

#include <Aonir/Platform/Win32/Utils/LastError.hpp>
#include <Aonir/Platform/Win32/Utils/StringConversion.hpp>

#include "WindowHandle.hpp"
#include "WindowListener.hpp"

namespace
{
    using namespace Aonir;

    auto GetListener(HWND window) -> Win32WindowListener &
    {
        auto data = GetWindowLongPtrW(window, GWLP_USERDATA);
        return *reinterpret_cast<Win32WindowListener *>(data); // NOLINT(performance-no-int-to-ptr)
    }

    auto SetListener(HWND window, LPARAM lparam) -> Win32WindowListener &
    {
        auto &settings = *reinterpret_cast<CREATESTRUCT *>(lparam); // NOLINT(performance-no-int-to-ptr)

        auto *data = settings.lpCreateParams;
        auto ptr = reinterpret_cast<LONG_PTR>(data);

        SetWindowLongPtrW(window, GWLP_USERDATA, ptr);

        return *static_cast<Win32WindowListener *>(data);
    }

    auto CALLBACK ProcessMessage(HWND window, UINT type, WPARAM wparam, LPARAM lparam) -> LRESULT
    {
        auto &listener = type == WM_CREATE ? SetListener(window, lparam) : GetListener(window);
        return listener.ProcessMessage(window, type, wparam, lparam);
    }
}

namespace Aonir
{
    auto Win32WindowClass::Deleter::operator()(LPCWSTR className) const -> void
    {
        [[maybe_unused]] auto success = UnregisterClassW(className, instance);
        assert(success == TRUE);
    }

    Win32WindowClass::Win32WindowClass(HINSTANCE instance, LPCWSTR className):
        m_instance(instance),
        m_className(className, Deleter{instance})
    {
    }

    auto Win32WindowClass::Instanciate(const WindowSettings &settings, WindowListener listener) -> Win32Window
    {
        auto options = DWORD(0);
        auto title = ToUtf16(settings.title);
        auto style = DWORD(WS_OVERLAPPEDWINDOW);
        auto x = static_cast<int>(settings.position.x);
        auto y = static_cast<int>(settings.position.y);
        auto width = static_cast<int>(settings.size.width);
        auto height = static_cast<int>(settings.size.height);
        auto *parent = HWND(nullptr);
        auto *menu = HMENU(nullptr);

        auto ptr = std::make_unique<Win32WindowListener>(std::move(listener));

        auto *window = CreateWindowExW(options, m_className.get(), title.c_str(), style, x, y, width, height, parent, menu, m_instance, ptr.get());

        if (window == nullptr)
        {
            throw Win32LastError("Failed to create window");
        }

        return {.handle = Win32WindowHandle(window), .listener = std::move(ptr)};
    }

    auto CreateWin32WindowClass(HINSTANCE instance, std::string_view name) -> Win32WindowClass
    {
        auto wname = ToUtf16(name);

        auto settings = WNDCLASSW{
            .style = 0,
            .lpfnWndProc = &ProcessMessage,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = instance,
            .hIcon = nullptr,
            .hCursor = nullptr,
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = wname.c_str(),
        };

        auto atom = RegisterClassW(&settings);

        if (atom == 0)
        {
            throw Win32LastError("Failed to create window class");
        }

        const auto *className = reinterpret_cast<const wchar_t *>(atom); // NOLINT(performance-no-int-to-ptr)

        return Win32WindowClass(instance, className);
    }
}
