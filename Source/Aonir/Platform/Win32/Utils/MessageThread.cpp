#include "MessageThread.hpp"

#include <cassert>
#include <functional>
#include <future>
#include <thread>
#include <utility>

#include <Windows.h> // NOLINT(misc-include-cleaner)

#include <WinUser.h>
#include <minwindef.h>
#include <processthreadsapi.h>

#include "LastError.hpp"

namespace
{
    using namespace Aonir;

    constexpr UINT RunTaskMessage = WM_USER;

    auto RunTask(WPARAM wparam) -> void
    {
        const auto &task = *reinterpret_cast<const TaskMessage *>(wparam); // NOLINT(performance-no-int-to-ptr)
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

            if (message.message == RunTaskMessage)
            {
                RunTask(message.wParam);
            }
        }
    }
}

namespace Aonir
{
    Win32MessageThread::Win32MessageThread(DWORD id, std::jthread thread):
        m_id(id),
        m_thread(std::move(thread))
    {
    }

    Win32MessageThread::~Win32MessageThread()
    {
        if (m_thread.joinable())
        {
            [[maybe_unused]] auto success = PostThreadMessageW(m_id, WM_QUIT, 0, 0);
            assert(success == TRUE);
        }
    }

    auto Win32MessageThread::Start(const TaskMessage &task) const -> void
    {
        auto wparam = reinterpret_cast<WPARAM>(&task);

        auto success = PostThreadMessageW(m_id, RunTaskMessage, wparam, 0);

        if (success == FALSE)
        {
            throw Win32LastError("Failed to post message to GUI thread");
        }
    }

    auto StartWin32MessageThread() -> Win32MessageThread
    {
        auto promise = std::promise<void>();
        auto future = promise.get_future();

        auto thread = std::jthread([&]
        {
            try
            {
                CreateMessageQueue();
                promise.set_value();
                RunMessageLoop();
            }
            catch (...)
            {
                assert(false);
            }
        });

        future.get();

        auto id = GetThreadId(thread.native_handle());

        return Win32MessageThread(id, std::move(thread));
    }
}
