#pragma once

#include <functional>
#include <vector>

#include "GpuDevice.hpp"

namespace Aonir
{
    struct NativeGpuFactory
    {
        void *ptr;
    };

    struct GpuFactory
    {
        NativeGpuFactory native;
        std::vector<GpuAdapter> availableAdapters;
        std::function<auto(const GpuDeviceSettings &)->GpuDevice> createDevice;
    };
}
