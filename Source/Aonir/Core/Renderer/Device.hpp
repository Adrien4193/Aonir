#pragma once

#include <compare>
#include <cstdint>
#include <functional>
#include <string>

namespace Aonir
{
    struct NativePhysicalDeviceHandle
    {
        void *ptr;

        auto operator<=>(const NativePhysicalDeviceHandle &) const = default;
    };

    enum class PhysicalDeviceType : std::uint8_t
    {
        IntegratedGpu,
        DiscreteGpu,
        VirtualGpu,
        Cpu,
    };

    struct PhysicalDevice
    {
        NativePhysicalDeviceHandle nativeHandle;
        std::string name;
        PhysicalDeviceType type;
    };

    struct DeviceSettings
    {
        NativePhysicalDeviceHandle physicalDevice;
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
