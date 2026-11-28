#include "Surface.hpp"

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_win32.h>

#include <Aonir/Vulkan/Core/Error.hpp>
#include <Aonir/Vulkan/Core/Surface.hpp>

namespace Aonir
{
    auto CreateWin32VulkanSurface(const Win32VulkanSurfaceSettings &settings) -> VulkanSurface
    {
        const auto surfaceInfo = VkWin32SurfaceCreateInfoKHR{
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .hinstance = settings.win32Instance,
            .hwnd = settings.window,
        };

        auto *surface = VkSurfaceKHR();

        const auto result = vkCreateWin32SurfaceKHR(settings.vulkanInstance, &surfaceInfo, nullptr, &surface);
        ThrowIfVulkanError("Cannot create Win32 Vulkan surface", result);

        return VulkanSurface{settings.vulkanInstance, surface};
    }
}
