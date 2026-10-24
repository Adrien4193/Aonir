#pragma once

#include <any>
#include <functional>
#include <string>

#include <Aonir/Core/Utils/Version.hpp>

namespace Aonir
{
    struct GraphicsApiSettings
    {
        std::string applicationName;
        Version applicationVersion;
        std::string engineName;
        Version engineVersion;
    };

    struct GraphicsApi
    {
        std::any test;
    };
}
