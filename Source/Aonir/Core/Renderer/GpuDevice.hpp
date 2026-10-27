#pragma once

#include <compare>
#include <functional>
#include <string>

namespace Aonir
{
    struct NativeGpuAdapter
    {
        void *ptr;

        auto operator<=>(const NativeGpuAdapter &) const = default;
    };

    struct GpuAdapter
    {
        NativeGpuAdapter native;
        std::string name;
    };

    struct GpuDeviceSettings
    {
        NativeGpuAdapter adapter;
    };

    struct NativeGpuDevice
    {
        void *ptr;

        auto operator<=>(const NativeGpuDevice &) const = default;
    };

    struct GpuDevice
    {
        NativeGpuDevice native;
    };
}
