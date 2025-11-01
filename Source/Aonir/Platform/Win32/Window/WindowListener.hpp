#pragma once

#include <functional>

#include <Windows.h>

#include <Aonir/Core/Window/WindowEvents.hpp>

namespace Aonir
{
    using WindowListener = std::function<auto(WindowEvent)->void>;

    class Win32WindowListener
    {
    private:
        WindowListener m_listener;

    public:
        explicit Win32WindowListener(WindowListener listener);

        auto ProcessMessage(HWND window, UINT type, WPARAM wparam, LPARAM lparam) -> LRESULT;
    };
}
