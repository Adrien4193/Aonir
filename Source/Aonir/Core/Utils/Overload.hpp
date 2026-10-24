#pragma once

namespace Aonir
{
    template<typename... Ts>
    struct Overload : Ts... // NOLINT(misc-multiple-inheritance)
    {
        using Ts::operator()...;
    };
}
