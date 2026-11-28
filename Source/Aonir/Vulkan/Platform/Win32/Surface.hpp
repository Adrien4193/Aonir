#pragma once

#include <Windows.h>

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_win32.h>

#include <Aonir/Vulkan/Core/Surface.hpp>

namespace Aonir
{
    constexpr const auto *khronosWin32ExtensionName = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;

    struct Win32VulkanSurfaceSettings
    {
        VkInstance vulkanInstance;
        HINSTANCE win32Instance;
        HWND window;
    };

    AONIR_VULKAN_API auto CreateWin32VulkanSurface(const Win32VulkanSurfaceSettings &settings) -> VulkanSurface;
}
