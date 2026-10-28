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
        explicit Win32WindowClass(HINSTANCE instance, LPCWSTR className);

        auto Instanciate(const WindowSettings &settings, WindowListener listener) -> Win32Window;
    };

    auto CreateWin32WindowClass(HINSTANCE instance, std::string_view name) -> Win32WindowClass;
}
