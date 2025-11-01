#include <Aonir/Platform/WindowFactory.hpp>

#include <cassert>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>
#include <libloaderapi.h>
#include <minwindef.h>

#include <Aonir/Core/Window/Window.hpp>
#include <Aonir/Core/Window/WindowEvents.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

#include <Aonir/Platform/Win32/Utils/MessageThread.hpp>
#include <Aonir/Platform/Win32/Window/WindowClass.hpp>
#include <Aonir/Platform/Win32/Window/WindowHandle.hpp>

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
            auto lock = std::scoped_lock(m_mutex);
            m_events.push_back(std::move(event));
        }

        auto Poll() -> std::vector<WindowEvent>
        {
            auto lock = std::scoped_lock(m_mutex);
            return std::exchange(m_events, {});
        }
    };

    class WindowHolder
    {
    private:
        std::reference_wrapper<Win32MessageThread> m_thread;
        std::optional<Win32Window> m_window;

    public:
        explicit WindowHolder(Win32MessageThread &thread, Win32Window window):
            m_thread(thread),
            m_window(std::move(window))
        {
        }

        WindowHolder(const WindowHolder &) = delete;
        WindowHolder(WindowHolder &&) = delete;

        ~WindowHolder()
        {
            try
            {
                m_thread.get().Run([this] { m_window.reset(); });
            }
            catch (...)
            {
                assert(false);
            }
        }

        auto operator=(const WindowHolder &) -> WindowHolder & = delete;
        auto operator=(WindowHolder &&) -> WindowHolder & = delete;

        auto GetHandle() -> Win32WindowHandle &
        {
            return m_window->handle; // NOLINT(bugprone-unchecked-optional-access)
        }
    };

    auto Instanciate(Win32MessageThread &thread, Win32WindowClass &windowClass, const WindowSettings &settings) -> WindowHandle
    {
        auto events = std::make_shared<EventBuffer>();
        auto listener = [=](auto event) { events->Add(std::move(event)); };

        auto window = thread.Run([&] { return windowClass.Instanciate(settings, listener); });

        auto holder = std::make_shared<WindowHolder>(thread, std::move(window));

        return WindowHandle{
            .native = NativeWindowHandle{holder->GetHandle().GetNativeHandle()},
            .poll = [=] { return events->Poll(); },
            .show = [=] { holder->GetHandle().Show(SW_NORMAL); },
            .setTitle = [=](auto title) { holder->GetHandle().SetTitle(title); },
            .setPosition = [=](const auto &position) { holder->GetHandle().SetPosition(position); },
            .resize = [=](const auto &size) { holder->GetHandle().Resize(size); },
        };
    }

    struct Win32WindowFactory
    {
        Win32MessageThread thread;
        Win32WindowClass windowClass;
    };

    auto CreateWin32WindowFactory(HINSTANCE instance) -> WindowFactory
    {
        auto thread = StartWin32MessageThread();
        auto windowClass = CreateWin32WindowClass(instance, "Aonir");

        auto factory = Win32WindowFactory{
            .thread = std::move(thread),
            .windowClass = std::move(windowClass),
        };

        auto ptr = std::make_shared<Win32WindowFactory>(std::move(factory));

        return [=](const auto &settings) { return Instanciate(ptr->thread, ptr->windowClass, settings); };
    }
}

namespace Aonir
{
    auto CreateNativeWindowFactory() -> WindowFactory
    {
        auto *instance = GetModuleHandleW(nullptr);
        return CreateWin32WindowFactory(instance);
    }
}
