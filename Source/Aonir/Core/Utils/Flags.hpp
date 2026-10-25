#pragma once

#include <cstdint>

namespace Aonir
{
    constexpr auto HasFlag(auto value, auto flag) -> bool
    {
        return static_cast<std::uint32_t>(value) & static_cast<std::uint32_t>(flag);
    }

    constexpr auto CombineFlags(auto... flags) -> auto
    {
        return (static_cast<std::uint32_t>(flags) | ...);
    }
}
