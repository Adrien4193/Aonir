#pragma once

#include <compare>
#include <exception>
#include <string>

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
        std::uint32_t x;
        std::uint32_t y;

        auto operator<=>(const WindowPosition &) const = default;
    };

    struct WindowMove
    {
        WindowPosition position;
    };

    struct WindowSize
    {
        std::uint32_t width;
        std::uint32_t height;

        auto operator<=>(const WindowSize &) const = default;
    };

    struct WindowResize
    {
        WindowSize size;
    };

    struct WindowClose
    {
    };
}
