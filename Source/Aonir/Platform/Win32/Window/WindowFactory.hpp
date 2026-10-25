#pragma once

#include <Aonir/Core/Window/WindowManager.hpp>

namespace Aonir
{
    struct Win32WindowFactorySettings
    {
        std::string className;
    };

    AONIR_PLATFORM_API auto CreateWin32WindowFactory(const Win32WindowFactorySettings &settings) -> WindowFactory;
}
