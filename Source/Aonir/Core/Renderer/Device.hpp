#pragma once

#include <compare>
#include <cstdint>
#include <functional>
#include <string>

namespace Aonir
{
    struct NativeAdapterHandle
    {
        void *ptr;

        auto operator<=>(const NativeAdapterHandle &) const = default;
    };

    enum class AdapterType : std::uint8_t
    {
        IntegratedGpu,
        DiscreteGpu,
        VirtualGpu,
        Cpu,
    };

    struct Adapter
    {
        NativeAdapterHandle nativeHandle;
        std::string name;
        AdapterType type;
    };

    struct DeviceSettings
    {
        NativeAdapterHandle adapter;
    };

    struct NativeDeviceHandle
    {
        void *ptr;

        auto operator<=>(const NativeDeviceHandle &) const = default;
    };

    struct Device
    {
        NativeDeviceHandle nativeHandle;
    };
}
