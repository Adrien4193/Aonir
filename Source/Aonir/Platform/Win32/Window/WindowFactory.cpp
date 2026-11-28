#include "WindowFactory.hpp"

#include <cassert>
#include <memory>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>

#include <Aonir/Core/Window/Window.hpp>
#include <Aonir/Core/Window/WindowEvent.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

#include <Aonir/Platform/Win32/Utils/StringConversion.hpp>

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
        if (!m_window)
        {
            return;
        }

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
        const auto wtitle = ToUtf16(settings.title);
        auto win32Settings = Win32WindowSettings{
            .title = wtitle.c_str(),
            .position = settings.position,
            .size = settings.size,
            .parent = nullptr,
            .options = 0,
            .style = WS_OVERLAPPEDWINDOW,
            .menu = nullptr,
        };

        const auto events = std::make_shared<EventBuffer>();
        const auto listener = [=](auto x) { events->Add(std::move(x)); };

        auto window = m_windowThread.Run([&] { return m_windowClass.Instanciate(win32Settings, listener); });
        const auto nativeWindow = NativeWindow(window.GetHandle());

        const auto handle = std::make_shared<Win32WindowHandle>(m_windowThread, std::move(window));

        return WindowHandle{
            .native = nativeWindow,
            .poll = [=] { return events->Poll(); },
            .show = [=] { handle->GetWindow().Show(); },
            .setTitle = [=](auto x) { handle->GetWindow().SetTitle(ToUtf16(x).c_str()); },
            .setPosition = [=](const auto &x) { handle->GetWindow().SetPosition(x); },
            .resize = [=](const auto &x) { handle->GetWindow().Resize(x); },
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
