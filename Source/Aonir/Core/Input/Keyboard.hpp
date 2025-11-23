#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>

namespace Aonir
{
    struct Key
    {
        std::size_t id;

        auto operator<=>(const Key &) const = default;
    };
}
