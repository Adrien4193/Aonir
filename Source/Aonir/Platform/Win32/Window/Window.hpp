#pragma once

#include <memory>
#include <type_traits>

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

        std::unique_ptr<std::remove_pointer_t<HWND>, Deleter> m_handle;
        std::unique_ptr<WindowListener> m_listener;

    public:
        AONIR_PLATFORM_API explicit Win32Window(HWND handle, std::unique_ptr<WindowListener> listener);

        [[nodiscard]] AONIR_PLATFORM_API auto GetHandle() const -> HWND;
        AONIR_PLATFORM_API auto Show() -> void;
        AONIR_PLATFORM_API auto SetTitle(LPCWSTR title) -> void;
        AONIR_PLATFORM_API auto SetPosition(const WindowPosition &position) -> void;
        AONIR_PLATFORM_API auto Resize(const WindowSize &size) -> void;
    };
}
