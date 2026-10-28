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
            auto operator()(HWND handle) const -> void;
        };

        std::unique_ptr<HWND__, Deleter> m_handle;
        std::unique_ptr<WindowListener> m_listener;

    public:
        explicit Win32Window(HWND handle, std::unique_ptr<WindowListener> listener);

        [[nodiscard]] auto GetNativeWindow() const -> HWND;
        auto Show() -> void;
        auto SetTitle(std::string_view title) -> void;
        auto SetPosition(const WindowPosition &position) -> void;
        auto Resize(const WindowSize &size) -> void;
    };
}
