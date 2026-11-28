#pragma once

#include <memory>
#include <type_traits>

#include <Windows.h>

#include <Aonir/Core/Window/WindowEvent.hpp>

#include "Window.hpp"

namespace Aonir
{
    struct Win32WindowClassSettings
    {
        HINSTANCE instance;
        LPCWSTR name;
    };

    struct Win32WindowSettings
    {
        LPCWSTR title;
        WindowPosition position;
        WindowSize size;
        HWND parent;
        DWORD options;
        DWORD style;
        HMENU menu;
    };

    class Win32WindowClass
    {
    private:
        struct Deleter
        {
            HINSTANCE instance;

            auto operator()(LPCWSTR className) const -> void;
        };

        std::unique_ptr<std::remove_pointer_t<LPCWSTR>, Deleter> m_className;

    public:
        AONIR_PLATFORM_API explicit Win32WindowClass(HINSTANCE instance, LPCWSTR className);

        AONIR_PLATFORM_API auto Instanciate(const Win32WindowSettings &settings, WindowListener listener) -> Win32Window;
    };

    AONIR_PLATFORM_API auto CreateWin32WindowClass(const Win32WindowClassSettings &settings) -> Win32WindowClass;
}
