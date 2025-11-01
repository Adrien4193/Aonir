#pragma once

#include <Aonir/Core/Window/WindowManager.hpp>

namespace Aonir
{
    AONIR_PLATFORM_API auto CreateNativeWindowFactory() -> WindowFactory;
}
