#pragma once

#include <compare>
#include <string>

namespace Aonir
{
    struct WindowPosition
    {
        std::size_t x;
        std::size_t y;

        auto operator<=>(const WindowPosition &) const = default;
    };

    struct WindowSize
    {
        std::size_t width;
        std::size_t height;

        auto operator<=>(const WindowSize &) const = default;
    };

    struct WindowSettings
    {
        std::string title;
        WindowPosition position;
        WindowSize size;
    };
}
