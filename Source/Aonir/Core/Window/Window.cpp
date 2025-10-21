#include "Window.hpp"

#include <string>
#include <string_view>
#include <vector>

#include "WindowEvents.hpp"

namespace Aonir
{
    auto CreateWindowBuffer(const WindowSettings &settings) -> WindowBuffer
    {
        return WindowBuffer{
            .title = settings.title,
            .position = settings.position,
            .size = settings.size,
            .events = {},
        };
    }

    Window::Window(const WindowPrivate &window):
        m_window(&window)
    {
    }

    auto Window::GetNativeHandle() const -> NativeWindowHandle
    {
        return m_window->handle.native;
    }

    auto Window::Show() -> void
    {
        m_window->handle.show();
    }

    auto Window::GetTitle() const -> const std::string &
    {
        return m_window->buffer.title;
    }

    auto Window::SetTitle(std::string_view title) -> void
    {
        m_window->handle.setTitle(title);
    }

    auto Window::GetPosition() const -> const WindowPosition &
    {
        return m_window->buffer.position;
    }

    auto Window::SetPosition(const WindowPosition &position) -> void
    {
        m_window->handle.setPosition(position);
    }

    auto Window::GetSize() const -> const WindowSize &
    {
        return m_window->buffer.size;
    }

    auto Window::Resize(const WindowSize &size) -> void
    {
        m_window->handle.resize(size);
    }

    auto Window::GetEvents() const -> const std::vector<WindowEvent> &
    {
        return m_window->buffer.events;
    }
}
