#pragma once

#include <string>
#include <vector>

#include <Aonir/Core/Renderer/GraphicsApi.hpp>
#include <Aonir/Core/Utils/Version.hpp>

namespace Aonir
{
    struct VulkanExtension
    {
        std::string name;
        VersionNumber version;
    };

    struct VulkanValidationLayer
    {
        std::string name;
        std::string description;
        VersionNumber specificationVersion;
        VersionNumber implementationVersion;
    };

    struct GraphicsApiSettings
    {
        std::string applicationName;
        Version applicationVersion;
        std::string engineName;
        Version engineVersion;
        std::vector<VulkanExtension> extensions;
        std::vector<VulkanValidationLayer> layers;
    };

    AONIR_VULKAN_API auto GetAvailableVulkanExtensions() -> std::vector<VulkanExtension>;
    AONIR_VULKAN_API auto GetAvailableVulkanValidationLayers() -> std::vector<VulkanValidationLayer>;
    AONIR_VULKAN_API auto CreateVulkanGraphicsApi(const GraphicsApiSettings &settings) -> GraphicsApi;
}
