#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace Aonir
{
    struct NativeGpuAdapter
    {
        void *ptr;
    };

    enum class GpuAdapterType : std::uint8_t
    {
        IntegratedGpu,
        DiscreteGpu,
        VirtualGpu,
        Cpu,
    };

    struct GpuAdapter
    {
        NativeGpuAdapter native;
        std::string name;
        std::optional<GpuAdapterType> type;
    };

    struct GpuDeviceSettings
    {
        NativeGpuAdapter adapter;
        std::uint32_t graphicsQueueCount;
        std::uint32_t computeQueueCount;
        std::uint32_t copyQueueCount;
    };

    struct NativeGpuDevice
    {
        void *ptr;
    };

    struct GpuDevice
    {
        NativeGpuDevice native;
    };
}
