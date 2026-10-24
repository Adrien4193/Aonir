#pragma once

#include <cstdint>
#include <optional>

#include <Aonir/Core/Utils/Unicode.hpp>

namespace Aonir
{
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
}
