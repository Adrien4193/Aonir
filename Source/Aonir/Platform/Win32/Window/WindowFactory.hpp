#pragma once

#include <optional>

#include <Aonir/Core/Window/WindowManager.hpp>

#include "WindowClass.hpp"
#include "WindowThread.hpp"

namespace Aonir
{
    class Win32WindowHandle
    {
    private:
        Win32WindowThread *m_windowThread;
        std::optional<Win32Window> m_window;

    public:
        AONIR_PLATFORM_API explicit Win32WindowHandle(Win32WindowThread &windowThread, Win32Window window);
        AONIR_PLATFORM_API ~Win32WindowHandle();

        Win32WindowHandle(const Win32WindowHandle &) = default;
        Win32WindowHandle(Win32WindowHandle &&) = default;

        auto operator=(const Win32WindowHandle &) -> Win32WindowHandle = delete;
        auto operator=(Win32WindowHandle &&) -> Win32WindowHandle = delete;

        AONIR_PLATFORM_API auto GetWindow() -> Win32Window &;
    };

    class Win32WindowFactory
    {
    private:
        Win32WindowThread m_windowThread;
        Win32WindowClass m_windowClass;

    public:
        AONIR_PLATFORM_API explicit Win32WindowFactory(Win32WindowThread windowThread, Win32WindowClass windowClass);

        AONIR_PLATFORM_API auto CreateWindowHandle(const WindowSettings &settings) -> WindowHandle;
    };

    AONIR_PLATFORM_API auto CreateWin32WindowFactory(const Win32WindowClassSettings &settings) -> WindowFactory;
}
