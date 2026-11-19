#include "Device.hpp"

#include <vulkan/vulkan_core.h>

namespace Aonir
{
    VulkanDevice::VulkanDevice(VkDevice handle):
        m_handle(handle)
    {
    }

    auto VulkanDevice::GetHandle() const -> VkDevice
    {
        return m_handle.get();
    }
}
