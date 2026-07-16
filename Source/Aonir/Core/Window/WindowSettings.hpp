#pragma once

#include <compare>
#include <cstdint>
#include <string>

namespace Aonir
{
    struct WindowPosition
    {
        std::uint32_t x;
        std::uint32_t y;

        auto operator<=>(const WindowPosition &) const = default;
    };

    struct WindowSize
    {
        std::uint32_t width;
        std::uint32_t height;

        auto operator<=>(const WindowSize &) const = default;
    };

    struct WindowSettings
    {
        std::string title;
        WindowPosition position;
        WindowSize size;
    };
}
