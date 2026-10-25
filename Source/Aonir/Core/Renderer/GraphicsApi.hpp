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
    struct NativeGraphicsApiHandle
    {
        void *ptr;

        auto operator<=>(const NativeGraphicsApiHandle &) const = default;
    };

    struct GraphicsApi
    {
        NativeGraphicsApiHandle nativeHandle;
        std::function<auto()->std::vector<PhysicalDevice>> getAvailablePhysicalDevices;
    };
}
