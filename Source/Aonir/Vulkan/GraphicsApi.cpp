#include "GraphicsApi.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <ranges>
#include <vector>

#include <vulkan/vulkan_core.h>

#include <Aonir/Core/Renderer/GraphicsApi.hpp>

#include "Utils.hpp"

namespace
{
    using namespace Aonir;

    using VulkanInstance = std::unique_ptr<VkInstance_T, decltype([](auto *x) { vkDestroyInstance(x, nullptr); })>;

    auto GetAvailableExtensions() -> std::vector<GraphicsApiExtension>
    {
        auto extensionCount = std::uint32_t();
        auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        ThrowIfVulkanError("Cannot get number of Vulkan extensions", result);

        auto availableExtensions = std::vector<VkExtensionProperties>(static_cast<std::size_t>(extensionCount));

        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
        ThrowIfVulkanError("Cannot enumerate Vulkan extensions", result);

        const auto toEngine = [](const auto &x) { return GraphicsApiExtension{.name = x.extensionName, .version = x.specVersion}; };

        return availableExtensions | std::views::transform(toEngine) | std::ranges::to<std::vector>();
    }

    auto GetAvailableValidationLayers() -> std::vector<GraphicsApiDebugLayer>
    {
        auto layerCount = std::uint32_t();

        auto result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        ThrowIfVulkanError("Cannot get number of Vulkan validation layers", result);

        auto availableLayers = std::vector<VkLayerProperties>(static_cast<std::size_t>(layerCount));

        result = vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        ThrowIfVulkanError("Cannot enumerate Vulkan validation layers", result);

        const auto toEngine = [](const auto &x)
        {
            return GraphicsApiDebugLayer{
                .name = x.layerName,
                .description = x.description,
                .specificationVersion = x.specVersion,
                .implementationVersion = x.implementationVersion,
            };
        };

        return availableLayers | std::views::transform(toEngine) | std::ranges::to<std::vector>();
    }

    auto CreateInstance(const GraphicsApiSettings &settings) -> GraphicsApi
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

        const auto getName = [](const auto &x) { return x.name.c_str(); };

        auto extensions = settings.extensions | std::views::transform(getName) | std::ranges::to<std::vector>();
        auto layers = settings.debugLayers | std::views::transform(getName) | std::ranges::to<std::vector>();

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

        return {std::make_shared<VkInstance>(instance)};
    }
}

namespace Aonir
{
    auto CreateVulkanFactory() -> GraphicsApiFactory
    {
        return {
            .getAvailableExtensions = [] { return GetAvailableExtensions(); },
            .getAvailableDebugLayers = [] { return GetAvailableValidationLayers(); },
            .createGraphicsApi = [](const auto &x) { return CreateInstance(x); },
        };
    }
}
