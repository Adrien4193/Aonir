#include "MessageThread.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <thread>
#include <utility>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinBase.h>
#include <WinUser.h>
#include <handleapi.h>
#include <minwindef.h>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <winnt.h>

#include "LastError.hpp"

namespace
{
    using namespace Aonir;

    constexpr UINT StartTaskMessage = WM_USER;

    auto Destroy(HANDLE handle) -> void
    {
        auto success = CloseHandle(handle);
        assert(success == TRUE);
    }

    using EventHandle = std::unique_ptr<void, decltype([](HANDLE handle) { Destroy(handle); })>;

    auto CreateEventHandle() -> EventHandle
    {
        auto *handle = CreateEventW(nullptr, FALSE, FALSE, nullptr);

        if (handle == nullptr)
        {
            throw LastError("Failed to create event");
        }

        return EventHandle(handle);
    }

    auto Wait(HANDLE event) -> void
    {
        auto code = WaitForSingleObjectEx(event, INFINITE, FALSE);

        if (code != WAIT_OBJECT_0)
        {
            throw LastError("Failed to wait for event");
        }
    }

    auto Notify(HANDLE event) -> void
    {
        auto success = SetEvent(event);

        if (success == FALSE)
        {
            throw LastError("Failed to set event");
        }
    }

    auto RunTask(WPARAM wparam) -> void
    {
        const auto &task = *reinterpret_cast<const std::function<void()> *>(wparam); // NOLINT(performance-no-int-to-ptr)
        task();
    }

    auto CreateMessageQueue() -> void
    {
        auto message = MSG();
        PeekMessageW(&message, nullptr, WM_USER, WM_USER, PM_NOREMOVE);
    }

    auto RunMessageLoop() -> void
    {
        auto message = MSG();

        while (true)
        {
            auto code = GetMessageW(&message, nullptr, 0, 0);

            if (code <= 0)
            {
                return;
            }

            TranslateMessage(&message);

            if (message.hwnd != nullptr)
            {
                DispatchMessageW(&message);
                continue;
            }

            if (message.message == StartTaskMessage)
            {
                RunTask(message.wParam);
            }
        }
    }
}

namespace Aonir
{
    MessageThread::MessageThread(DWORD id, std::jthread thread):
        m_id(id),
        m_thread(std::move(thread))
    {
    }

    MessageThread::~MessageThread()
    {
        if (m_thread.joinable())
        {
            auto success = PostThreadMessageW(m_id, WM_QUIT, 0, 0);
            assert(success == TRUE);
        }
    }

    auto MessageThread::Start(const std::function<void()> &task) const -> void
    {
        auto wparam = reinterpret_cast<WPARAM>(&task);
        auto success = PostThreadMessageW(m_id, StartTaskMessage, wparam, 0);

        if (success == FALSE)
        {
            throw LastError("Failed to post message to GUI thread");
        }
    }

    auto StartMessageThread() -> MessageThread
    {
        auto event = CreateEventHandle();
        auto *ptr = event.get();

        auto thread = std::jthread([=]
        {
            try
            {
                CreateMessageQueue();
                Notify(ptr);
                RunMessageLoop();
            }
            catch (...)
            {
                assert(false);
            }
        });

        Wait(ptr);

        auto id = GetThreadId(thread.native_handle());

        return MessageThread(id, std::move(thread));
    }
}
