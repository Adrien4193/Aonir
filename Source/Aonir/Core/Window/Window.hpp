#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "WindowEvents.hpp"

namespace Aonir
{
    struct WindowSettings
    {
        std::string title;
        WindowPosition position;
        WindowSize size;
    };

    struct NativeWindowHandle
    {
        void *ptr;

        auto operator<=>(const NativeWindowHandle &) const = default;
    };

    struct WindowHandle
    {
        NativeWindowHandle native;
        std::function<auto()->std::vector<WindowEvent>> poll;
        std::function<auto()->void> show;
        std::function<auto(std::string_view)->void> setTitle;
        std::function<auto(const WindowPosition &)->void> setPosition;
        std::function<auto(const WindowSize &)->void> resize;
    };

    struct WindowBuffer
    {
        std::string title;
        WindowPosition position;
        WindowSize size;
        std::vector<WindowEvent> events;
    };

    AONIR_CORE_API auto CreateWindowBuffer(const WindowSettings &settings) -> WindowBuffer;

    struct WindowPrivate
    {
        WindowHandle handle;
        WindowBuffer buffer;
    };

    class Window
    {
    private:
        const WindowPrivate *m_window;

    public:
        AONIR_CORE_API explicit Window(const WindowPrivate &window);

        AONIR_CORE_API auto GetNativeHandle() const -> NativeWindowHandle;
        AONIR_CORE_API auto Show() -> void;
        AONIR_CORE_API auto GetTitle() const -> const std::string &;
        AONIR_CORE_API auto SetTitle(std::string_view title) -> void;
        AONIR_CORE_API auto GetPosition() const -> const WindowPosition &;
        AONIR_CORE_API auto SetPosition(const WindowPosition &position) -> void;
        AONIR_CORE_API auto GetSize() const -> const WindowSize &;
        AONIR_CORE_API auto Resize(const WindowSize &size) -> void;
        AONIR_CORE_API auto GetEvents() const -> const std::vector<WindowEvent> &;
    };
}
