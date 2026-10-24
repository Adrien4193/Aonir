#pragma once

#include <Aonir/Core/Renderer/GraphicsApi.hpp>

namespace Aonir
{
    AONIR_VULKAN_API auto CreateVulkanFactory() -> GraphicsApiFactory;
}
