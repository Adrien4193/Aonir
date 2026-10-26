#pragma once

#include <compare>
#include <cstdint>
#include <functional>
#include <string>

namespace Aonir
{
    struct NativeAdapter
    {
        void *ptr;

        auto operator<=>(const NativeAdapter &) const = default;
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
        NativeAdapter native;
        std::string name;
        AdapterType type;
    };

    struct DeviceSettings
    {
        NativeAdapter adapter;
    };

    struct NativeDevice
    {
        void *ptr;

        auto operator<=>(const NativeDevice &) const = default;
    };

    struct Device
    {
        NativeDevice native;
    };
}
