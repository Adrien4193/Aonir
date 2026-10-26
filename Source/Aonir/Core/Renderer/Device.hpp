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

    struct Adapter
    {
        NativeAdapter native;
        std::string name;
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
