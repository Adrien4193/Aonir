#pragma once

#include <exception>
#include <string>
#include <variant>

#include <Aonir/Core/Input/Keyboard.hpp>
#include <Aonir/Core/Input/Mouse.hpp>

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
        KeyPress,
        KeyRelease,
        TextInput>;
}
