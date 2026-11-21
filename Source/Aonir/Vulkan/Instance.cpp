#include "Instance.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#include <Aonir/Core/Utils/Exception.hpp>
#include <Aonir/Core/Utils/Flags.hpp>
#include <Aonir/Core/Utils/Logger.hpp>

#include "Device.hpp"
#include "Error.hpp"

namespace
{
    using namespace Aonir;

    auto GetLogLevel(VkDebugUtilsMessageSeverityFlagBitsEXT severity) -> LogLevel
    {
        if (HasFlag(severity, VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT))
        {
            return LogLevel::Error;
        }

        if (HasFlag(severity, VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT))
        {
            return LogLevel::Warn;
        }

        if (HasFlag(severity, VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT))
        {
            return LogLevel::Info;
        }

        if (HasFlag(severity, VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT))
        {
            return LogLevel::Debug;
        }

        return LogLevel::Off;
    }

    VKAPI_ATTR auto VKAPI_CALL LoggerCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
        void *userData) -> VkBool32
    {
        (void)type;

        auto &logger = *static_cast<Logger *>(userData);
        const auto level = GetLogLevel(severity);

        logger.Log(level, "{}", callbackData->pMessage);

        return VK_FALSE;
    }

    auto CreateDebugMessenger(VkInstance instance, Logger &logger) -> VulkanDebugMessenger
    {
        auto createMessenger =
            reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));

        auto destroyMessenger =
            reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

        if (createMessenger == nullptr || destroyMessenger == nullptr)
        {
            throw Exception("Vulkan debug extension must be enabled to use a custom logger");
        }

        const auto debugCreateInfo = VkDebugUtilsMessengerCreateInfoEXT{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = CombineFlags(
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT),
            .messageType = CombineFlags(
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT),
            .pfnUserCallback = LoggerCallback,
            .pUserData = &logger,
        };

        auto *messenger = VkDebugUtilsMessengerEXT();

        const auto result = createMessenger(instance, &debugCreateInfo, nullptr, &messenger);
        ThrowIfVulkanError("Cannot create Vulkan debug messenger", result);

        return VulkanDebugMessenger{messenger, {.instance = instance, .destroyMessenger = destroyMessenger}};
    }
}

namespace Aonir
{
    auto GetAvailableVulkanExtensions() -> std::vector<VulkanExtension>
    {
        auto extensionCount = std::uint32_t();

        auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        ThrowIfVulkanError("Cannot get number of available Vulkan extensions", result);

        auto availableExtensions = std::vector<VkExtensionProperties>(static_cast<std::size_t>(extensionCount));

        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
        ThrowIfVulkanError("Cannot enumerate available Vulkan extensions", result);

        const auto toExtension = [](const auto &x)
        {
            return VulkanExtension{
                .name = x.extensionName,
            };
        };

        return availableExtensions | std::views::transform(toExtension) | std::ranges::to<std::vector>();
    }

    auto GetAvailableVulkanValidationLayers() -> std::vector<VulkanValidationLayer>
    {
        auto layerCount = std::uint32_t();

        auto result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        ThrowIfVulkanError("Cannot get number of Vulkan validation layers", result);

        auto availableLayers = std::vector<VkLayerProperties>(static_cast<std::size_t>(layerCount));

        result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        ThrowIfVulkanError("Cannot enumerate Vulkan validation layers", result);

        const auto toLayer = [](const auto &x)
        {
            return VulkanValidationLayer{
                .name = x.layerName,
                .description = x.description,
            };
        };

        return availableLayers | std::views::transform(toLayer) | std::ranges::to<std::vector>();
    }

    VulkanInstance::VulkanInstance(VkInstance handle, std::optional<VulkanDebugMessenger> messenger):
        m_handle(handle),
        m_messenger(std::move(messenger))
    {
    }

    auto VulkanInstance::GetHandle() const -> VkInstance
    {
        return m_handle.get();
    }

    auto VulkanInstance::GetAvailablePhysicalDevices() -> std::vector<VulkanPhysicalDevice>
    {
        auto *instance = m_handle.get();

        auto deviceCount = std::uint32_t();

        auto result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        ThrowIfVulkanError("Cannot get number of Vulkan physical devices", result);

        auto availableDevices = std::vector<VkPhysicalDevice>(static_cast<std::size_t>(deviceCount));

        result = vkEnumeratePhysicalDevices(instance, &deviceCount, availableDevices.data());
        ThrowIfVulkanError("Cannot enumerate Vulkan physical devices", result);

        const auto toDevice = [](auto x) { return VulkanPhysicalDevice(x); };

        return availableDevices | std::views::transform(toDevice) | std::ranges::to<std::vector>();
    }

    auto CreateVulkanInstance(const VulkanInstanceSettings &settings) -> VulkanInstance
    {
        auto [major, minor, patch] = settings.applicationVersion;
        auto [engineMajor, engineMinor, enginePatch] = settings.engineVersion;

        const auto applicationInfo = VkApplicationInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = settings.applicationName,
            .applicationVersion = VK_MAKE_VERSION(major, minor, patch),
            .pEngineName = settings.engineName,
            .engineVersion = VK_MAKE_VERSION(engineMajor, engineMinor, enginePatch),
            .apiVersion = VK_API_VERSION_1_4,
        };

        const auto instanceInfo = VkInstanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = static_cast<std::uint32_t>(settings.validationLayers.size()),
            .ppEnabledLayerNames = settings.validationLayers.data(),
            .enabledExtensionCount = static_cast<std::uint32_t>(settings.extensions.size()),
            .ppEnabledExtensionNames = settings.extensions.data(),
        };

        auto *instance = VkInstance();

        const auto result = vkCreateInstance(&instanceInfo, nullptr, &instance);
        ThrowIfVulkanError("Cannot create Vulkan instance", result);

        if (settings.logger == nullptr)
        {
            return VulkanInstance(instance, std::nullopt);
        }

        auto messenger = CreateDebugMessenger(instance, *settings.logger);
        return VulkanInstance(instance, std::move(messenger));
    }
}
