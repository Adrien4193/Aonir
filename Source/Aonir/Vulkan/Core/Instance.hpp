#pragma once

#include <memory>
#include <optional>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

#include <vulkan/vulkan.h>

#include <Aonir/Core/Utils/Logger.hpp>
#include <Aonir/Core/Utils/Version.hpp>

#include "Device.hpp"

namespace Aonir
{
    struct VulkanExtension
    {
        std::string name;
    };

    constexpr const auto *vulkanDebugExtensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    constexpr const auto *khronosSurfaceExtensionName = VK_KHR_SURFACE_EXTENSION_NAME;

    AONIR_VULKAN_API auto GetAvailableVulkanExtensions() -> std::vector<VulkanExtension>;

    struct VulkanValidationLayer
    {
        std::string name;
        std::string description;
    };

    constexpr const auto *khronosValidationLayer = "VK_LAYER_KHRONOS_validation";

    AONIR_VULKAN_API auto GetAvailableVulkanValidationLayers() -> std::vector<VulkanValidationLayer>;

    struct VulkanDebugMessengerDeleter
    {
        VkInstance instance;
        PFN_vkDestroyDebugUtilsMessengerEXT destroyMessenger;

        auto operator()(VkDebugUtilsMessengerEXT messenger) const -> void
        {
            destroyMessenger(instance, messenger, nullptr);
        }
    };

    using VulkanDebugMessenger = std::unique_ptr<std::remove_pointer_t<VkDebugUtilsMessengerEXT>, VulkanDebugMessengerDeleter>;

    struct VulkanInstanceSettings
    {
        const char *applicationName;
        Version applicationVersion;
        const char *engineName;
        Version engineVersion;
        std::span<const char *const> extensions;
        std::span<const char *const> validationLayers;
        Logger *logger;
    };

    class VulkanInstance
    {
    private:
        std::unique_ptr<std::remove_pointer_t<VkInstance>, decltype([](auto *x) { vkDestroyInstance(x, nullptr); })> m_handle;
        std::optional<VulkanDebugMessenger> m_messenger;

    public:
        AONIR_VULKAN_API explicit VulkanInstance(VkInstance handle, std::optional<VulkanDebugMessenger> messenger);

        [[nodiscard]] AONIR_VULKAN_API auto GetHandle() const -> VkInstance;
        [[nodiscard]] AONIR_VULKAN_API auto GetAvailablePhysicalDevices() -> std::vector<VulkanPhysicalDevice>;
    };

    AONIR_VULKAN_API auto CreateVulkanInstance(const VulkanInstanceSettings &settings) -> VulkanInstance;
}
