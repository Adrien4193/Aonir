#include "Device.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

#include <Aonir/Core/Renderer/GpuDevice.hpp>

#include "Error.hpp"

namespace
{
    using namespace Aonir;

    auto GetAdapterType(VkPhysicalDeviceType type) -> std::optional<GpuAdapterType>
    {
        switch (type)
        {
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return GpuAdapterType::IntegratedGpu;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return GpuAdapterType::DiscreteGpu;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return GpuAdapterType::VirtualGpu;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return GpuAdapterType::Cpu;
        default:
            return std::nullopt;
        }
    }
}

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

    auto VulkanDevice::GetQueue(std::uint32_t family, std::uint32_t index) const -> VkQueue
    {
        auto queue = VkQueue();
        vkGetDeviceQueue(m_handle.get(), family, index, &queue);
        return queue;
    }

    VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice handle):
        m_handle(handle)
    {
    }

    auto VulkanPhysicalDevice::GetHandle() const -> VkPhysicalDevice
    {
        return m_handle;
    }

    auto VulkanPhysicalDevice::GetProperties() const -> VulkanPhysicalDeviceProperties
    {
        auto extensionCount = std::uint32_t();

        auto result = vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, nullptr);
        ThrowIfVulkanError("Cannot get number of available Vulkan physical device extensions", result);

        auto availableExtensions = std::vector<VkExtensionProperties>(static_cast<std::size_t>(extensionCount));

        result = vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &extensionCount, availableExtensions.data());
        ThrowIfVulkanError("Cannot enumerate available Vulkan physical device extensions", result);

        auto properties = VkPhysicalDeviceProperties();
        vkGetPhysicalDeviceProperties(m_handle, &properties);

        auto queueFamilyCount = std::uint32_t();
        vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &queueFamilyCount, nullptr);

        auto queueFamilies = std::vector<VkQueueFamilyProperties>(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &queueFamilyCount, queueFamilies.data());

        return {
            .availableExtensions = availableExtensions,
            .properties = properties,
            .queueFamilies = std::move(queueFamilies),
        };
    }

    auto VulkanPhysicalDevice::CreateDevice(const VulkanDeviceSettings &settings) -> VulkanDevice
    {
        const auto toCreateInfo = [](const auto &x)
        {
            return VkDeviceQueueCreateInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = x.family,
                .queueCount = static_cast<std::uint32_t>(x.priorities.size()),
                .pQueuePriorities = x.priorities.data(),
            };
        };

        const auto queues = settings.queues | std::views::transform(toCreateInfo) | std::ranges::to<std::vector>();

        const auto deviceInfo = VkDeviceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = static_cast<std::uint32_t>(queues.size()),
            .pQueueCreateInfos = queues.data(),
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = static_cast<std::uint32_t>(settings.extensions.size()),
            .ppEnabledExtensionNames = settings.extensions.data(),
            .pEnabledFeatures = &settings.features,
        };

        auto *device = VkDevice();

        const auto result = vkCreateDevice(m_handle, &deviceInfo, nullptr, &device);
        ThrowIfVulkanError("Cannot create Vulkan logical device", result);

        return VulkanDevice{device};
    }
}
