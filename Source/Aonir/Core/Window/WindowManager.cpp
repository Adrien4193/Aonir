#include "WindowManager.hpp"

#include <cassert>
#include <string>
#include <utility>
#include <variant>

#include <Aonir/Core/Utils/Overload.hpp>

#include "Window.hpp"
#include "WindowEvent.hpp"

namespace Aonir
{
    WindowManager::WindowManager(WindowFactory factory):
        m_factory(std::move(factory))
    {
    }

    auto WindowManager::Add(const WindowSettings &settings) -> Window
    {
        auto handle = m_factory(settings);
        const auto nativeWindow = handle.native;

        auto window = WindowPrivate{
            .handle = std::move(handle),
            .title = settings.title,
            .position = settings.position,
            .size = settings.size,
            .events = {},
        };

        auto [i, inserted] = m_windows.emplace(nativeWindow, std::move(window));
        assert(inserted);

        return Window(i->second);
    }

    auto WindowManager::Remove(Window window) -> void
    {
        m_windows.erase(window.GetNativeWindow());
    }

    auto WindowManager::Poll() -> void
    {
        for (auto &[_, window] : m_windows)
        {
            window.events = window.handle.poll();

            auto processEvent = Overload{
                [&](const WindowRename &e) { window.title = e.title; },
                [&](const WindowMove &e) { window.position = e.position; },
                [&](const WindowResize &e) { window.size = e.size; },
                [](const auto &) {},
            };

            for (const auto &event : window.events)
            {
                std::visit(processEvent, event);
            }
        }
    }
}
