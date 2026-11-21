#pragma once

#include <memory>
#include <span>
#include <type_traits>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace Aonir
{
    struct VulkanPhysicalDevice
    {
        VkPhysicalDevice handle;
        std::vector<VkExtensionProperties> availableExtensions;
        VkPhysicalDeviceProperties properties;
        std::vector<VkQueueFamilyProperties> queueFamilies;
    };

    struct VulkanDeviceSettings
    {
        VkPhysicalDevice physicalDevice;
        std::span<const char *const> extensions;
        std::span<const VkDeviceQueueCreateInfo> queues;
        VkPhysicalDeviceFeatures features;
    };

    class VulkanDevice
    {
    private:
        std::unique_ptr<std::remove_pointer_t<VkDevice>, decltype([](VkDevice device) { vkDestroyDevice(device, nullptr); })> m_handle;

    public:
        AONIR_VULKAN_API explicit VulkanDevice(VkDevice handle);

        [[nodiscard]] AONIR_VULKAN_API auto GetHandle() const -> VkDevice;
    };
}
