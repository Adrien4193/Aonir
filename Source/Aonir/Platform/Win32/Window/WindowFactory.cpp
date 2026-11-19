#include "WindowFactory.hpp"

#include <cassert>
#include <memory>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>

#include <Aonir/Core/Window/Window.hpp>
#include <Aonir/Core/Window/WindowEvent.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

#include "Window.hpp"
#include "WindowClass.hpp"
#include "WindowThread.hpp"

namespace
{
    using namespace Aonir;

    class EventBuffer
    {
    private:
        std::mutex m_mutex;
        std::vector<WindowEvent> m_events;

    public:
        auto Add(WindowEvent event) -> void
        {
            const auto lock = std::scoped_lock(m_mutex);
            m_events.push_back(std::move(event));
        }

        auto Poll() -> std::vector<WindowEvent>
        {
            const auto lock = std::scoped_lock(m_mutex);
            return std::exchange(m_events, {});
        }
    };
}

namespace Aonir
{
    Win32WindowHandle::Win32WindowHandle(Win32WindowThread &windowThread, Win32Window window):
        m_windowThread(&windowThread),
        m_window(std::move(window))
    {
    }

    Win32WindowHandle::~Win32WindowHandle()
    {
        try
        {
            m_windowThread->Run([this] { m_window.reset(); });
        }
        catch (...)
        {
            assert(false);
        }
    }

    auto Win32WindowHandle::GetWindow() -> Win32Window &
    {
        return *m_window; // NOLINT(bugprone-unchecked-optional-access)
    }

    Win32WindowFactory::Win32WindowFactory(Win32WindowThread windowThread, Win32WindowClass windowClass):
        m_windowThread(std::move(windowThread)),
        m_windowClass(std::move(windowClass))
    {
    }

    auto Win32WindowFactory::CreateWindowHandle(const WindowSettings &settings) -> WindowHandle
    {
        const auto events = std::make_shared<EventBuffer>();
        const auto listener = [=](auto x) { events->Add(std::move(x)); };

        auto window = m_windowThread.Run([&] { return m_windowClass.Instanciate(settings, listener); });
        const auto nativeWindow = NativeWindow(window.GetHandle());

        const auto handle = std::make_shared<Win32WindowHandle>(m_windowThread, std::move(window));

        return WindowHandle{
            .native = nativeWindow,
            .poll = [=] { return events->Poll(); },
            .show = [=] { handle->GetWindow().Show(); },
            .setTitle = [=](auto title) { handle->GetWindow().SetTitle(title); },
            .setPosition = [=](const auto &position) { handle->GetWindow().SetPosition(position); },
            .resize = [=](const auto &size) { handle->GetWindow().Resize(size); },
        };
    }

    auto CreateWin32WindowFactory(const Win32WindowClassSettings &settings) -> WindowFactory
    {
        auto windowThread = StartWin32WindowThread();
        auto windowClass = CreateWin32WindowClass(settings);
        const auto factory = std::make_shared<Win32WindowFactory>(std::move(windowThread), std::move(windowClass));
        return [=](const auto &x) { return factory->CreateWindowHandle(x); };
    }
}
