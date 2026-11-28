#pragma once

#include <memory>

#include <vulkan/vulkan_core.h>

namespace Aonir
{
    class VulkanSurface
    {
    private:
        struct Deleter
        {
            VkInstance instance;

            auto operator()(VkSurfaceKHR surface) const
            {
                vkDestroySurfaceKHR(instance, surface, nullptr);
            }
        };

        std::unique_ptr<std::remove_pointer_t<VkSurfaceKHR>, Deleter> m_handle;

    public:
        AONIR_VULKAN_API explicit VulkanSurface(VkInstance instance, VkSurfaceKHR surface);

        [[nodiscard]] AONIR_VULKAN_API auto GetHandle() const -> VkSurfaceKHR;
    };
}
