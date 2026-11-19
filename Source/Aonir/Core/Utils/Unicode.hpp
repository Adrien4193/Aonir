#pragma once

#include <cstdint>
#include <format>
#include <utility>

namespace Aonir
{
    enum class Codepoint : std::uint32_t
    {
    };
}

// NOLINTBEGIN(readability-identifier-naming)

namespace std
{
    template<>
    struct formatter<Aonir::Codepoint>
    {
        static constexpr auto parse(std::format_parse_context &context)
        {
            return context.begin();
        }

        static auto format(Aonir::Codepoint codepoint, std::format_context &context)
        {
            return std::format_to(context.out(), "U+{:04X}", std::to_underlying(codepoint));
        }
    };
}

// NOLINTEND(readability-identifier-naming)
