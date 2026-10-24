#pragma once

#include <cstdint>

namespace Aonir
{
    struct Version
    {
        std::uint32_t major;
        std::uint32_t minor;
        std::uint32_t patch;
    };

    using VersionNumber = std::uint32_t;
}
