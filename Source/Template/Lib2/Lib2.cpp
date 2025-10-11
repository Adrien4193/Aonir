#include "Lib2.hpp"

#include <Template/Lib1/Lib1.hpp>

namespace Template
{
    auto Lib2(int value) -> int
    {
        return value + Lib1(value);
    }
}
