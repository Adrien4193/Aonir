#pragma once

#include <memory>
#include <span>
#include <type_traits>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace Aonir
{
    struct VulkanQueueSettings
    {
        std::uint32_t family;
        std::span<const float> priorities;
    };

    struct VulkanDeviceSettings
    {
        std::span<const char *const> extensions;
        std::span<const VulkanQueueSettings> queues;
        VkPhysicalDeviceFeatures features;
    };

    class VulkanDevice
    {
    private:
        std::unique_ptr<std::remove_pointer_t<VkDevice>, decltype([](VkDevice device) { vkDestroyDevice(device, nullptr); })> m_handle;

    public:
        AONIR_VULKAN_API explicit VulkanDevice(VkDevice handle);

        [[nodiscard]] AONIR_VULKAN_API auto GetHandle() const -> VkDevice;
        [[nodiscard]] AONIR_VULKAN_API auto GetQueue(std::uint32_t family, std::uint32_t index) const -> VkQueue;
    };

    struct VulkanPhysicalDeviceProperties
    {
        std::vector<VkExtensionProperties> availableExtensions;
        VkPhysicalDeviceProperties properties;
        std::vector<VkQueueFamilyProperties> queueFamilies;
    };

    class VulkanPhysicalDevice
    {
    private:
        VkPhysicalDevice m_handle;

    public:
        AONIR_VULKAN_API explicit VulkanPhysicalDevice(VkPhysicalDevice handle);

        [[nodiscard]] AONIR_VULKAN_API auto GetHandle() const -> VkPhysicalDevice;
        [[nodiscard]] AONIR_VULKAN_API auto GetProperties() const -> VulkanPhysicalDeviceProperties;
        [[nodiscard]] AONIR_VULKAN_API auto CreateDevice(const VulkanDeviceSettings &settings) -> VulkanDevice;
    };
}
