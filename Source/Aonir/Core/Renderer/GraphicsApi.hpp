#pragma once

#include <compare>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "Device.hpp"

namespace Aonir
{
    struct NativeGraphicsApi
    {
        void *ptr;

        auto operator<=>(const NativeGraphicsApi &) const = default;
    };

    struct GraphicsApi
    {
        NativeGraphicsApi native;
        std::function<auto()->std::vector<Adapter>> getAvailableAdapters;
    };
}
