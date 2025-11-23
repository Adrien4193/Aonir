#pragma once

#include <compare>
#include <cstddef>

namespace Aonir
{
    struct MouseButton
    {
        std::size_t id;

        auto operator<=>(const MouseButton &) const = default;
    };

    constexpr auto leftMouseButton = MouseButton{0};
    constexpr auto rightMouseButton = MouseButton{1};
    constexpr auto middleMouseButton = MouseButton{2};

    struct MousePosition
    {
        std::size_t x;
        std::size_t y;

        auto operator<=>(const MousePosition &) const = default;
    };
}
