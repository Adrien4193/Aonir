#pragma once

#include <concepts>
#include <functional>
#include <future>
#include <thread>
#include <type_traits>

#include <Windows.h>

namespace Aonir
{

    class Win32MessageThread
    {
    public:
        using Task = std::function<auto()->void>;

    private:
        DWORD m_id;
        std::jthread m_thread;

    public:
        explicit Win32MessageThread(DWORD id, std::jthread thread);
        ~Win32MessageThread();

        Win32MessageThread(const Win32MessageThread &) = delete;
        auto operator=(const Win32MessageThread &) -> Win32MessageThread & = delete;

        Win32MessageThread(Win32MessageThread &&) = default;
        auto operator=(Win32MessageThread &&) -> Win32MessageThread & = default;

        auto Run(std::invocable<> auto &&task) const -> decltype(task())
        {
            using ResultType = decltype(task());

            auto promise = std::promise<ResultType>();
            auto future = promise.get_future();

            auto wrapper = [&]
            {
                try
                {
                    if constexpr (std::is_void_v<ResultType>)
                    {
                        task();
                        promise.set_value();
                    }
                    else
                    {
                        promise.set_value(task());
                    }
                }
                catch (...)
                {
                    promise.set_exception(std::current_exception());
                }
            };

            Start(wrapper);

            return future.get();
        }

    private:
        auto Start(const Task &task) const -> void;
    };

    auto StartWin32MessageThread() -> Win32MessageThread;
}
