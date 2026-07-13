#pragma once

#include <compare>
#include <cstdint>

namespace Aonir
{
    enum class MouseButton : std::uint8_t
    {
        Left,
        Right,
        Middle,
    };

    struct MousePosition
    {
        std::uint32_t x;
        std::uint32_t y;

        auto operator<=>(const MousePosition &) const = default;
    };

    struct MouseScrollDelta
    {
        std::int32_t x;
        std::int32_t y;

        auto operator<=>(const MouseScrollDelta &) const = default;
    };
}
