#pragma once

#include <functional>
#include <memory>
#include <string_view>

#include <Windows.h>

#include <Aonir/Core/Window/Window.hpp>

#include "Window.hpp"
#include "WindowListener.hpp"

namespace Aonir
{
    class Win32WindowClass
    {
    private:
        struct Deleter
        {
            HINSTANCE instance;

            auto operator()(LPCWSTR className) const -> void;
        };

        HINSTANCE m_instance;
        std::unique_ptr<const wchar_t, Deleter> m_className;

    public:
        AONIR_PLATFORM_API explicit Win32WindowClass(HINSTANCE instance, LPCWSTR className);

        AONIR_PLATFORM_API auto Instanciate(const WindowSettings &settings, WindowListener listener) -> Win32Window;
    };

    AONIR_PLATFORM_API auto CreateWin32WindowClass(HINSTANCE instance, std::string_view name) -> Win32WindowClass;
}
