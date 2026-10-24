#include "GraphicsApi.hpp"

#include <memory>

#include <vulkan/vulkan.h>

#include <Aonir/Core/Renderer/GraphicsApi.hpp>

#include "Utils.hpp"

namespace
{
    using VulkanInstance = std::unique_ptr<VkInstance_T, decltype([](auto *x) { vkDestroyInstance(x, nullptr); })>;
}

namespace Aonir
{
    auto CreateVulkanGraphicsApi(const GraphicsApiSettings &settings) -> GraphicsApi
    {
        auto [major, minor, patch] = settings.applicationVersion;
        auto [engineMajor, engineMinor, enginePatch] = settings.engineVersion;

        auto applicationInfo = VkApplicationInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = settings.applicationName.c_str(),
            .applicationVersion = VK_MAKE_VERSION(major, minor, patch),
            .pEngineName = settings.engineName.c_str(),
            .engineVersion = VK_MAKE_VERSION(engineMajor, engineMinor, enginePatch),
            .apiVersion = VK_API_VERSION_1_4,
        };

        auto instanceInfo = VkInstanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr,
        };

        VkInstance instance;

        auto result = vkCreateInstance(&instanceInfo, nullptr, &instance);
        ThrowIfVulkanError("Cannot create Vulkan instance", result);

        return {std::make_shared<VkInstance>(instance)};
    }
}
