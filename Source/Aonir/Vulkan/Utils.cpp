#include "Utils.hpp"

#include <format>

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

#include "Utils.hpp"

namespace Aonir
{
    VulkanException::VulkanException(std::string message, VkResult result):
        Exception(std::move(message)),
        m_result(result)
    {
    }

    auto VulkanException::GetResult() const -> VkResult
    {
        return m_result;
    }

    auto ThrowIfVulkanError(const char *message, VkResult result) -> void
    {
        if (result == VK_SUCCESS)
        {
            return;
        }

        auto error = std::format("{}: {}", message, string_VkResult(result));
        throw VulkanException(std::move(error), result);
    }
}
