#pragma once

namespace Aonir
{
    template<typename... Ts>
    struct Overload
    {
        using Ts::operator()...;
    };
}
