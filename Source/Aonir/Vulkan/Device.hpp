#pragma once

#include <memory>
#include <type_traits>

#include <vulkan/vulkan_core.h>

namespace Aonir
{
    struct VulkanDeviceSettings
    {
        VkPhysicalDevice physicalDevice;
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
