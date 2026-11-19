#pragma once

#include <memory>
#include <string_view>
#include <type_traits>

#include <Windows.h>

#include <Aonir/Core/Window/Window.hpp>

#include "Window.hpp"
#include "WindowListener.hpp"

namespace Aonir
{
    struct Win32WindowClassSettings
    {
        std::string_view name;
    };

    class Win32WindowClass
    {
    private:
        struct Deleter
        {
            HINSTANCE instance;

            auto operator()(LPCWSTR className) const -> void;
        };

        HINSTANCE m_instance;
        std::unique_ptr<std::remove_pointer_t<LPCWSTR>, Deleter> m_className;

    public:
        AONIR_PLATFORM_API explicit Win32WindowClass(HINSTANCE instance, LPCWSTR className);

        AONIR_PLATFORM_API auto Instanciate(const WindowSettings &settings, WindowListener listener) -> Win32Window;
    };

    AONIR_PLATFORM_API auto CreateWin32WindowClass(const Win32WindowClassSettings &settings) -> Win32WindowClass;
}
