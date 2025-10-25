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

    constexpr UINT StartTaskMessage = WM_USER;

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

        return MessageThread(id, std::move(thread));
    }
}
