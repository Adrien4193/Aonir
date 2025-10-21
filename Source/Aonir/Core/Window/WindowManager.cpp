#include "WindowManager.hpp"

#include <cassert>
#include <string>
#include <utility>
#include <variant>

#include <Aonir/Core/Utils/Overload.hpp>

#include "Window/Window.hpp"
#include "Window/WindowEvents.hpp"

namespace Aonir
{
    WindowManager::WindowManager(WindowFactory factory):
        m_factory(std::move(factory))
    {
    }

    auto WindowManager::Add(const WindowSettings &settings) -> Window
    {
        auto handle = m_factory(settings);

        auto buffer = CreateWindowBuffer(settings);

        auto window = WindowPrivate{
            .handle = std::move(handle),
            .buffer = std::move(buffer),
        };

        auto [i, inserted] = m_windows.emplace(window.handle.native, std::move(window));
        assert(inserted);

        return Window(i->second);
    }

    auto WindowManager::Remove(Window window) -> void
    {
        m_windows.erase(window.GetNativeHandle());
    }

    auto WindowManager::Poll() -> void
    {
        for (auto &[_, window] : m_windows)
        {
            window.buffer.events = window.handle.poll();

            auto processEvent = Overload{
                [](const WindowError &) {},
                [&](const WindowRename &e) { window.buffer.title = e.title; },
                [&](const WindowMove &e) { window.buffer.position = e.position; },
                [&](const WindowResize &e) { window.buffer.size = e.size; },
                [](const WindowClose &) {},
                [](const auto &) {},
            };

            for (const auto &event : window.buffer.events)
            {
                std::visit(processEvent, event);
            }
        }
    }
}
