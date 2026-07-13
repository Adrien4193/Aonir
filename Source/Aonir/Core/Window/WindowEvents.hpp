#pragma once

#include <exception>
#include <string>
#include <variant>

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "WindowSettings.hpp"

namespace Aonir
{
    struct WindowError
    {
        std::exception_ptr exception;
    };

    struct WindowRename
    {
        std::string title;
    };

    struct WindowMove
    {
        WindowPosition position;
    };

    struct WindowResize
    {
        WindowSize size;
    };

    struct WindowClose
    {
    };

    struct MouseButtonPress
    {
        MouseButton button;
    };

    struct MouseButtonRelease
    {
        MouseButton button;
    };

    struct MouseMove
    {
        MousePosition position;
    };

    struct MouseScroll
    {
        MouseScrollDelta delta;
    };

    struct KeyPress
    {
        Key key;
    };

    struct KeyRelease
    {
        Key key;
    };

    struct TextInput
    {
        std::uint32_t codepoint;
    };

    using WindowEvent = std::variant<
        WindowError,
        WindowRename,
        WindowMove,
        WindowResize,
        WindowClose,
        MouseButtonPress,
        MouseButtonRelease,
        MouseMove,
        MouseScroll,
        KeyPress,
        KeyRelease,
        TextInput>;
}
