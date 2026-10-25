#include "GraphicsApi.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#include <Aonir/Core/Renderer/Device.hpp>
#include <Aonir/Core/Renderer/GraphicsApi.hpp>
#include <Aonir/Core/Utils/Exception.hpp>
#include <Aonir/Core/Utils/Flags.hpp>
#include <Aonir/Core/Utils/Logger.hpp>

#include "Utils.hpp"

namespace
{
    using namespace Aonir;

    using VulkanInstance = std::unique_ptr<VkInstance_T, decltype([](auto *x) { vkDestroyInstance(x, nullptr); })>;

    auto CreateVulkanInstance(const VulkanSettings &settings) -> VulkanInstance
    {
        auto [major, minor, patch] = settings.applicationVersion;
        auto [engineMajor, engineMinor, enginePatch] = settings.engineVersion;

        const auto applicationInfo = VkApplicationInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = settings.applicationName.c_str(),
            .applicationVersion = VK_MAKE_VERSION(major, minor, patch),
            .pEngineName = settings.engineName.c_str(),
            .engineVersion = VK_MAKE_VERSION(engineMajor, engineMinor, enginePatch),
            .apiVersion = VK_API_VERSION_1_4,
        };

        const auto toCString = [](const auto &x) { return x.c_str(); };

        auto extensions = settings.extensions | std::views::transform(toCString) | std::ranges::to<std::vector>();
        auto layers = settings.validationLayers | std::views::transform(toCString) | std::ranges::to<std::vector>();

        const auto instanceInfo = VkInstanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = static_cast<std::uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        auto *instance = VkInstance();

        const auto result = vkCreateInstance(&instanceInfo, nullptr, &instance);
        ThrowIfVulkanError("Cannot create Vulkan instance", result);

        return VulkanInstance{instance};
    }

    struct VulkanDebugMessengerDeleter
    {
        VkInstance instance;
        PFN_vkDestroyDebugUtilsMessengerEXT destroyMessenger;

        auto operator()(VkDebugUtilsMessengerEXT messenger) const
        {
            destroyMessenger(instance, messenger, nullptr);
        }
    };

    using VulkanDebugMessenger = std::unique_ptr<VkDebugUtilsMessengerEXT_T, VulkanDebugMessengerDeleter>;

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
            return LogLevel::Trace;
        }

        return LogLevel::Off;
    }

    VKAPI_ATTR auto VKAPI_CALL LoggerCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
        void *userData) -> VkBool32
    {
        auto &logger = *static_cast<Logger *>(userData);

        const auto level = GetLogLevel(severity);
        (void)type;

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

        return {messenger, {.instance = instance, .destroyMessenger = destroyMessenger}};
    }

    auto GetPhysicalDeviceType(VkPhysicalDeviceType type) -> std::optional<PhysicalDeviceType>
    {
        switch (type)
        {
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return PhysicalDeviceType::IntegratedGpu;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return PhysicalDeviceType::DiscreteGpu;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return PhysicalDeviceType::VirtualGpu;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return PhysicalDeviceType::Cpu;
        default:
            return std::nullopt;
        }
    }

    auto TryGetPhysicalDeviceProperties(VkPhysicalDevice device) -> std::optional<PhysicalDevice>
    {
        auto properties = VkPhysicalDeviceProperties();
        vkGetPhysicalDeviceProperties(device, &properties);

        auto type = GetPhysicalDeviceType(properties.deviceType);

        if (!type)
        {
            return std::nullopt;
        }

        return PhysicalDevice{
            .nativeHandle = NativePhysicalDeviceHandle(device),
            .name = properties.deviceName,
            .type = *type,
        };
    }

    auto GetAvailablePhysicalDevices(VkInstance instance) -> std::vector<PhysicalDevice>
    {
        auto deviceCount = std::uint32_t();

        auto result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        ThrowIfVulkanError("Cannot get number of Vulkan physical devices", result);

        auto availableDevices = std::vector<VkPhysicalDevice>(static_cast<std::size_t>(deviceCount));

        result = vkEnumeratePhysicalDevices(instance, &deviceCount, availableDevices.data());
        ThrowIfVulkanError("Cannot enumerate Vulkan physical devices", result);

        const auto tryGet = std::views::transform([](auto *x) { return TryGetPhysicalDeviceProperties(x); });
        const auto ifSupported = std::views::filter([](const auto &x) { return x.has_value(); });
        const auto andGet = std::views::transform([](auto &&x) { return std::move(*x); });

        return availableDevices | tryGet | ifSupported | andGet | std::ranges::to<std::vector>();
    }

    struct VulkanApi
    {
        VulkanInstance instance;
        VulkanDebugMessenger messenger;
    };
}

namespace Aonir
{
    auto GetVulkanDebugExtensionName() -> const char *
    {
        return VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }

    auto GetKhronosVulkanValidationLayerName() -> const char *
    {
        return "VK_LAYER_KHRONOS_validation";
    }

    auto GetAvailableVulkanExtensions() -> std::vector<VulkanExtension>
    {
        auto extensionCount = std::uint32_t();

        auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        ThrowIfVulkanError("Cannot get number of Vulkan extensions", result);

        auto availableExtensions = std::vector<VkExtensionProperties>(static_cast<std::size_t>(extensionCount));

        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
        ThrowIfVulkanError("Cannot enumerate Vulkan extensions", result);

        const auto toEngine = [](const auto &x) { return VulkanExtension{.name = x.extensionName, .version = x.specVersion}; };

        return availableExtensions | std::views::transform(toEngine) | std::ranges::to<std::vector>();
    }

    auto GetAvailableVulkanValidationLayers() -> std::vector<VulkanValidationLayer>
    {
        auto layerCount = std::uint32_t();

        auto result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        ThrowIfVulkanError("Cannot get number of Vulkan validation layers", result);

        auto availableLayers = std::vector<VkLayerProperties>(static_cast<std::size_t>(layerCount));

        result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        ThrowIfVulkanError("Cannot enumerate Vulkan validation layers", result);

        const auto toEngine = [](const auto &x)
        {
            return VulkanValidationLayer{
                .name = x.layerName,
                .description = x.description,
                .specificationVersion = x.specVersion,
                .implementationVersion = x.implementationVersion,
            };
        };

        return availableLayers | std::views::transform(toEngine) | std::ranges::to<std::vector>();
    }

    auto CreateVulkanGraphicsApi(const VulkanSettings &settings) -> GraphicsApi
    {
        auto instance = CreateVulkanInstance(settings);
        auto messenger = VulkanDebugMessenger();

        if (settings.logger != nullptr)
        {
            messenger = CreateDebugMessenger(instance.get(), *settings.logger);
        }

        auto api = VulkanApi{.instance = std::move(instance), .messenger = std::move(messenger)};
        const auto ptr = std::make_shared<VulkanApi>(std::move(api));

        return {
            .nativeHandle = NativeGraphicsApiHandle(ptr->instance.get()),
            .getAvailablePhysicalDevices = [=] { return GetAvailablePhysicalDevices(ptr->instance.get()); },
        };
    }
}
