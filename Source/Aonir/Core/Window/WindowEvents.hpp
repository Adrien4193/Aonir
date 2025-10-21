#pragma once

#include <exception>
#include <string>
#include <variant>

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

    struct WindowPosition
    {
        std::size_t x;
        std::size_t y;

        auto operator<=>(const WindowPosition &) const = default;
    };

    struct WindowMove
    {
        WindowPosition position;
    };

    struct WindowSize
    {
        std::size_t width;
        std::size_t height;

        auto operator<=>(const WindowSize &) const = default;
    };

    struct WindowResize
    {
        WindowSize size;
    };

    struct WindowClose
    {
    };

    using WindowEvent = std::variant<WindowError, WindowRename, WindowMove, WindowResize, WindowClose>;
}
