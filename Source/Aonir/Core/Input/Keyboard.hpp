#pragma once

#include <cstddef>
#include <cstdint>

namespace Aonir
{
    struct Key
    {
        std::size_t id;

        auto operator<=>(const Key &) const = default;
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
