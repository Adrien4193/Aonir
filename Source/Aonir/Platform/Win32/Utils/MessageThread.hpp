#pragma once

#include <concepts>
#include <functional>
#include <future>
#include <thread>
#include <type_traits>

#include <Windows.h>

namespace Aonir
{
    class MessageThread
    {
    private:
        DWORD m_id;
        std::jthread m_thread;

    public:
        explicit MessageThread(DWORD id, std::jthread thread);
        ~MessageThread();

        MessageThread(const MessageThread &) = delete;
        MessageThread(MessageThread &&) = default;

        auto operator=(const MessageThread &) -> MessageThread & = delete;
        auto operator=(MessageThread &&) -> MessageThread & = default;

        auto Run(std::invocable<> auto &&task) const -> decltype(task())
        {
            using ResultType = decltype(task());

            auto promise = std::promise<ResultType>();
            auto future = promise.get_future();

            auto wrapper = std::function<void()>([&]
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
            });

            Start(wrapper);

            return future.get();
        }

    private:
        auto Start(const std::function<void()> &task) const -> void;
    };

    auto StartMessageThread() -> MessageThread;
}
