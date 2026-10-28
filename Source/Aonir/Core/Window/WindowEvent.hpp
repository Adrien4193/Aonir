#pragma once

#include <compare>
#include <exception>
#include <functional>
#include <optional>
#include <string>
#include <variant>

#include <Aonir/Core/Utils/Unicode.hpp>

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
        std::int32_t x;
        std::int32_t y;

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

    enum class VirtualKey : std::uint8_t
    {
        Left,
        Right,
        Up,
        Down,
        Back,
        Delete,
        Return,
        Tab,
    };

    enum class PhysicalKey : std::uint8_t
    {
    };

    struct Key
    {
        std::optional<VirtualKey> virtualKey;
        PhysicalKey physicalKey;
    };

    struct KeyPress
    {
        Key key;
    };

    struct KeyRelease
    {
        Key key;
    };

    struct KeyText
    {
        Codepoint codepoint;
    };

    enum class MouseButton : std::uint8_t
    {
        Left,
        Right,
        Middle,
    };

    struct MouseButtonPress
    {
        MouseButton button;
    };

    struct MouseButtonRelease
    {
        MouseButton button;
    };

    struct MousePosition
    {
        std::int32_t x;
        std::int32_t y;

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
        KeyText>;

    using WindowListener = std::function<auto(WindowEvent)->void>;
}
