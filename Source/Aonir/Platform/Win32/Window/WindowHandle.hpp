#pragma once

#include <memory>
#include <string_view>

#include <Windows.h>

#include <Aonir/Core/Window/WindowEvents.hpp>

namespace Aonir
{
    class Win32WindowHandle
    {
    private:
        struct Deleter
        {
            auto operator()(HWND handle) const -> void;
        };

        std::unique_ptr<HWND__, Deleter> m_handle;

    public:
        explicit Win32WindowHandle(HWND handle);

        auto GetHandle() const -> HWND;
        auto Show(int code) -> void;
        auto SetTitle(std::string_view title) -> void;
        auto SetPosition(const WindowPosition &position) -> void;
        auto Resize(const WindowSize &size) -> void;
    };
}
