#include <gtest/gtest.h>

#include <Aonir/Core/Window/Window.hpp>
#include <Aonir/Core/Window/WindowEvents.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

using namespace Aonir;

namespace
{
    auto CreateFactory(NativeWindowHandle handle, WindowBuffer &buffer) -> WindowFactory
    {
        return [&, handle](const WindowSettings &)
        {
            return WindowHandle{
                .native = handle,
                .poll = [&] { return buffer.events; },
                .show = [&] {},
                .setTitle = [&](const auto &value) { buffer.title = value; },
                .setPosition = [&](const auto &value) { buffer.position = value; },
                .resize = [&](const auto &value) { buffer.size = value; },
            };
        };
    }
}

TEST(Window, SingleWindow) // NOLINT
{
    auto handle = NativeWindowHandle{nullptr};

    auto settings = WindowSettings{
        .title = "Test",
        .position = {.x = 1, .y = 2},
        .size = {.width = 3, .height = 4},
    };

    auto buffer = CreateWindowBuffer(settings);
    auto factory = CreateFactory(handle, buffer);
    auto windows = WindowManager(factory);

    auto window = windows.Add(settings);

    windows.Poll();

    ASSERT_EQ(window.GetNativeHandle(), handle);
    ASSERT_EQ(window.GetTitle(), buffer.title);
    ASSERT_EQ(window.GetPosition(), buffer.position);
    ASSERT_EQ(window.GetSize(), buffer.size);
    ASSERT_TRUE(window.GetEvents().empty());

    buffer.events.emplace_back(WindowRename{"Test1"});
    windows.Poll();
    ASSERT_EQ(window.GetTitle(), "Test1");

    buffer.events.emplace_back(WindowMove{{.x = 5, .y = 6}});
    windows.Poll();
    ASSERT_EQ(window.GetPosition().x, 5);
    ASSERT_EQ(window.GetPosition().y, 6);

    buffer.events.emplace_back(WindowResize{{.width = 8, .height = 9}});
    windows.Poll();
    ASSERT_EQ(window.GetSize().width, 8);
    ASSERT_EQ(window.GetSize().height, 9);
}
