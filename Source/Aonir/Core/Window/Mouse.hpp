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
        X1,
        X2,
    };

    struct MouseButtonUp
    {
        MouseButton button;
    };

    struct MouseButtonDown
    {
        MouseButton button;
    };

    struct MousePosition
    {
        std::uint32_t x;
        std::uint32_t y;

        auto operator<=>(const MousePosition &) const = default;
    };

    struct MouseMove
    {
        MousePosition position;
    };

    struct MouseScrollDelta
    {
        std::int32_t x;
        std::int32_t y;

        auto operator<=>(const MouseScrollDelta &) const = default;
    };

    struct MouseScroll
    {
        MouseScrollDelta delta;
    };
}
