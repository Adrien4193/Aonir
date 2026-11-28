#include "Surface.hpp"

#include <vulkan/vulkan_core.h>

namespace Aonir
{
    VulkanSurface::VulkanSurface(VkInstance instance, VkSurfaceKHR surface):
        m_handle(surface, Deleter{instance})
    {
    }

    auto VulkanSurface::GetHandle() const -> VkSurfaceKHR
    {
        return m_handle.get();
    }
}
