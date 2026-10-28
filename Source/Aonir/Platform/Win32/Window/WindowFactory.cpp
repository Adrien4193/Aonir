#include "WindowFactory.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <libloaderapi.h>

#include <Aonir/Core/Window/Window.hpp>
#include <Aonir/Core/Window/WindowEvent.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

#include <Aonir/Platform/Win32/Utils/MessageThread.hpp>

#include "Window.hpp"
#include "WindowClass.hpp"

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

    class Win32WindowHandle
    {
    private:
        std::reference_wrapper<Win32MessageThread> m_messageThread;
        std::optional<Win32Window> m_window;

    public:
        explicit Win32WindowHandle(Win32MessageThread &messageThread, Win32Window window):
            m_messageThread(messageThread),
            m_window(std::move(window))
        {
        }

        ~Win32WindowHandle()
        {
            try
            {
                m_messageThread.get().Run([this] { m_window.reset(); });
            }
            catch (...)
            {
                assert(false);
            }
        }

        Win32WindowHandle(const Win32WindowHandle &) = delete;
        Win32WindowHandle(Win32WindowHandle &&) = delete;

        auto operator=(const Win32WindowHandle &) -> Win32WindowHandle = delete;
        auto operator=(Win32WindowHandle &&) -> Win32WindowHandle = delete;

        auto GetWindow() -> Win32Window &
        {
            return *m_window; // NOLINT(bugprone-unchecked-optional-access)
        }
    };

    class Win32WindowFactory
    {
    private:
        Win32MessageThread m_messageThread;
        Win32WindowClass m_windowClass;

    public:
        explicit Win32WindowFactory(Win32MessageThread messageThread, Win32WindowClass windowClass):
            m_messageThread(std::move(messageThread)),
            m_windowClass(std::move(windowClass))
        {
        }

        ~Win32WindowFactory() = default;

        Win32WindowFactory(const Win32WindowFactory &) = delete;
        Win32WindowFactory(Win32WindowFactory &&) = delete;

        auto operator=(const Win32WindowFactory &) -> Win32WindowFactory = delete;
        auto operator=(Win32WindowFactory &&) -> Win32WindowFactory = delete;

        auto CreateWindowHandle(const WindowSettings &settings) -> WindowHandle
        {
            const auto events = std::make_shared<EventBuffer>();
            const auto listener = [=](auto x) { events->Add(std::move(x)); };

            auto window = m_messageThread.Run([&] { return m_windowClass.Instanciate(settings, listener); });
            const auto nativeWindow = NativeWindow(window.GetNativeWindow());

            const auto handle = std::make_shared<Win32WindowHandle>(m_messageThread, std::move(window));

            return WindowHandle{
                .native = nativeWindow,
                .poll = [=] { return events->Poll(); },
                .show = [=] { handle->GetWindow().Show(); },
                .setTitle = [=](auto title) { handle->GetWindow().SetTitle(title); },
                .setPosition = [=](const auto &position) { handle->GetWindow().SetPosition(position); },
                .resize = [=](const auto &size) { handle->GetWindow().Resize(size); },
            };
        }
    };
}

namespace Aonir
{
    auto CreateWin32WindowFactory(const Win32WindowManagerSettings &settings) -> WindowFactory
    {
        auto *instance = GetModuleHandleW(nullptr);

        auto messageThread = StartWin32MessageThread();
        auto windowClass = CreateWin32WindowClass(instance, settings.className);

        const auto factory = std::make_shared<Win32WindowFactory>(std::move(messageThread), std::move(windowClass));

        return [=](const auto &x) { return factory->CreateWindowHandle(x); };
    }
}
