#pragma once

#include <concepts>
#include <functional>
#include <future>
#include <thread>
#include <type_traits>

#include <Windows.h>

namespace Aonir
{
    class Win32WindowThread
    {
    public:
        using Task = std::function<auto()->void>;

    private:
        DWORD m_id;
        std::jthread m_thread;

    public:
        AONIR_PLATFORM_API explicit Win32WindowThread(DWORD id, std::jthread thread);
        AONIR_PLATFORM_API ~Win32WindowThread();

        Win32WindowThread(const Win32WindowThread &) = delete;
        Win32WindowThread(Win32WindowThread &&) = default;

        auto operator=(const Win32WindowThread &) -> Win32WindowThread & = delete;
        auto operator=(Win32WindowThread &&) -> Win32WindowThread & = default;

        auto Run(std::invocable<> auto &&task) const -> decltype(task())
        {
            using ResultType = decltype(task());

            auto promise = std::promise<ResultType>();
            auto future = promise.get_future();

            const auto runTaskAndSetFuture = [&]
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

            const auto wrapper = Task(runTaskAndSetFuture);

            Start(wrapper);

            return future.get();
        }

    private:
        auto Start(const Task &task) const -> void;
    };

    AONIR_PLATFORM_API auto StartWin32WindowThread() -> Win32WindowThread;
}
