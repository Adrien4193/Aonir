#pragma once

#include <compare>
#include <functional>
#include <vector>

#include "GpuDevice.hpp"

namespace Aonir
{
    struct NativeGpuFactory
    {
        void *ptr;

        auto operator<=>(const NativeGpuFactory &) const = default;
    };

    struct GpuFactory
    {
        NativeGpuFactory native;
        std::vector<GpuAdapter> availableAdapters;
        std::function<auto(const GpuDeviceSettings &)->GpuDevice> createDevice;
    };
}
