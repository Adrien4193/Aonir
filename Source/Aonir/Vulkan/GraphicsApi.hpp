#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <Aonir/Core/Renderer/GraphicsApi.hpp>
#include <Aonir/Core/Utils/Logger.hpp>
#include <Aonir/Core/Utils/Version.hpp>

namespace Aonir
{
    struct VulkanExtension
    {
        std::string name;
        std::uint32_t version;
    };

    struct VulkanValidationLayer
    {
        std::string name;
        std::string description;
        std::uint32_t specificationVersion;
        std::uint32_t implementationVersion;
    };

    struct VulkanSettings
    {
        std::string applicationName;
        Version applicationVersion;
        std::string engineName;
        Version engineVersion;
        std::vector<std::string> extensions;
        std::vector<std::string> validationLayers;
        Logger *logger;
    };

    AONIR_VULKAN_API auto GetVulkanDebugExtensionName() -> const char *;
    AONIR_VULKAN_API auto GetKhronosVulkanValidationLayerName() -> const char *;
    AONIR_VULKAN_API auto GetAvailableVulkanExtensions() -> std::vector<VulkanExtension>;
    AONIR_VULKAN_API auto GetAvailableVulkanValidationLayers() -> std::vector<VulkanValidationLayer>;
    AONIR_VULKAN_API auto CreateVulkanGraphicsApi(const VulkanSettings &settings) -> GraphicsApi;
}
