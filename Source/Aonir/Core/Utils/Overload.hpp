#pragma once

namespace Aonir
{
    template<typename... Ts>
    struct Overload : Ts...
    {
        using Ts::operator()...;
    };
}
