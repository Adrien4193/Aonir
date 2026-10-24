#pragma once

#include <cstdint>

namespace Aonir
{
    enum class Key : std::uint32_t
    {
    };

    struct KeyUp
    {
        Key key;
    };

    struct KeyDown
    {
        Key key;
    };

    struct TextInput
    {
        std::uint32_t codepoint;
    };
}
