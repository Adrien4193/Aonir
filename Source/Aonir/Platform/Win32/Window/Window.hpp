#pragma once

#include <memory>
#include <string_view>

#include <Windows.h>

#include <Aonir/Core/Window/WindowEvent.hpp>

namespace Aonir
{
    class Win32Window
    {
    private:
        struct Deleter
        {
            AONIR_PLATFORM_API auto operator()(HWND handle) const -> void;
        };

        std::unique_ptr<HWND__, Deleter> m_handle;
        std::unique_ptr<WindowListener> m_listener;

    public:
        AONIR_PLATFORM_API explicit Win32Window(HWND handle, std::unique_ptr<WindowListener> listener);

        AONIR_PLATFORM_API [[nodiscard]] auto GetNativeWindow() const -> HWND;
        AONIR_PLATFORM_API auto Show() -> void;
        AONIR_PLATFORM_API auto SetTitle(std::string_view title) -> void;
        AONIR_PLATFORM_API auto SetPosition(const WindowPosition &position) -> void;
        AONIR_PLATFORM_API auto Resize(const WindowSize &size) -> void;
    };
}
