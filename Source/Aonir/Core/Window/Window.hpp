#pragma once

#include <string>
#include <vector>

#include "WindowEvent.hpp"

namespace Aonir
{
    struct WindowSettings
    {
        const char *title;
        WindowPosition position;
        WindowSize size;
    };

    struct NativeWindow
    {
        void *ptr;
    };

    struct WindowHandle
    {
        NativeWindow native;
        std::function<auto()->std::vector<WindowEvent>> poll;
        std::function<auto()->void> show;
        std::function<auto(const char *)->void> setTitle;
        std::function<auto(const WindowPosition &)->void> setPosition;
        std::function<auto(const WindowSize &)->void> resize;
    };

    struct WindowPrivate
    {
        WindowHandle handle;
        std::string title;
        WindowPosition position;
        WindowSize size;
        std::vector<WindowEvent> events;
    };

    class Window
    {
    private:
        const WindowPrivate *m_window;

    public:
        AONIR_CORE_API explicit Window(const WindowPrivate &window);

        [[nodiscard]] AONIR_CORE_API auto GetNativeWindow() const -> NativeWindow;
        AONIR_CORE_API auto Show() -> void;
        [[nodiscard]] AONIR_CORE_API auto GetTitle() const -> const std::string &;
        AONIR_CORE_API auto SetTitle(const char *title) -> void;
        [[nodiscard]] AONIR_CORE_API auto GetPosition() const -> const WindowPosition &;
        AONIR_CORE_API auto SetPosition(const WindowPosition &position) -> void;
        [[nodiscard]] AONIR_CORE_API auto GetSize() const -> const WindowSize &;
        AONIR_CORE_API auto Resize(const WindowSize &size) -> void;
        [[nodiscard]] AONIR_CORE_API auto GetEvents() const -> const std::vector<WindowEvent> &;
    };
}
