#pragma once

#include <functional>
#include <map>
#include <string_view>

#include "Window.hpp"

namespace Aonir
{
    struct WindowFactorySettings
    {
        std::string_view className = "Aonir";
    };

    using WindowFactory = std::function<auto(const WindowSettings &)->WindowHandle>;

    class WindowManager
    {
    private:
        WindowFactory m_factory;
        std::map<NativeWindowHandle, WindowPrivate> m_windows;

    public:
        AONIR_CORE_API explicit WindowManager(WindowFactory factory);

        AONIR_CORE_API auto Add(const WindowSettings &settings) -> Window;
        AONIR_CORE_API auto Remove(Window window) -> void;
        AONIR_CORE_API auto Poll() -> void;
    };
}
