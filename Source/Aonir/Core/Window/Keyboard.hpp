#pragma once

#include <cstdint>

namespace Aonir
{
    enum class Key : std::uint32_t
    {
    };

    struct KeyPress
    {
        Key key;
    };

    struct KeyRelease
    {
        Key key;
    };

    struct TextInput
    {
        std::uint32_t codepoint;
    };
}
