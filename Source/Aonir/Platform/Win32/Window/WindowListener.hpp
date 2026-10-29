#pragma once

#include <functional>

#include <Windows.h>

#include <Aonir/Core/Window/WindowEvent.hpp>

namespace Aonir
{
    AONIR_PLATFORM_API auto ProcessWindowMessage(const WindowListener &listener, HWND window, UINT type, WPARAM wparam, LPARAM lparam) -> LRESULT;
}
