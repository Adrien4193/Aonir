#pragma once

#include <vulkan/vulkan_core.h>

#include <Aonir/Core/Utils/Exception.hpp>

namespace Aonir
{
    class VulkanException : public Exception
    {
    private:
        VkResult m_result;

    public:
        explicit VulkanException(std::string message, VkResult result);

        [[nodiscard]] AONIR_VULKAN_API auto GetResult() const -> VkResult;
    };

    AONIR_VULKAN_API auto ThrowIfVulkanError(const char *message, VkResult result) -> void;
}
