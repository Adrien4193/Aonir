#pragma once

#include <any>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include <Aonir/Core/Utils/Logger.hpp>
#include <Aonir/Core/Utils/Version.hpp>

namespace Aonir
{
    struct GraphicsApiExtension
    {
        std::string name;
        VersionNumber version;
    };

    struct GraphicsApiDebugLayer
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
        std::vector<GraphicsApiExtension> extensions;
        std::vector<GraphicsApiDebugLayer> debugLayers;
        std::reference_wrapper<Logger> logger;
    };

    struct GraphicsApi
    {
        std::any test;
    };

    struct GraphicsApiFactory
    {
        std::function<auto()->std::vector<GraphicsApiExtension>> getAvailableExtensions;
        std::function<auto()->std::vector<GraphicsApiDebugLayer>> getAvailableDebugLayers;
        std::function<auto(const GraphicsApiSettings &)->GraphicsApi> createGraphicsApi;
    };
}
