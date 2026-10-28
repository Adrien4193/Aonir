#include <gtest/gtest.h>

#include <vector>

#include <Aonir/Core/Window/Window.hpp>
#include <Aonir/Core/Window/WindowEvent.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

using namespace Aonir;

namespace
{
    struct MockWindow
    {
        WindowSettings settings;
        std::vector<WindowEvent> events;
        bool showed = false;
    };
}

TEST(Window, WindowManager) // NOLINT
{
    auto windows = std::vector<MockWindow>();

    const auto factory = [&](const auto &x)
    {
        const auto index = windows.size();
        windows.push_back(MockWindow{x});

        return WindowHandle{
            .native = NativeWindow{&windows[index]},
            .poll = [&windows, index] { return windows[index].events; },
            .show = [&windows, index] { windows[index].showed = true; },
            .setTitle = [&windows, index](auto x) { windows[index].settings.title = x; },
            .setPosition = [&windows, index](const auto &x) { windows[index].settings.position = x; },
            .resize = [&windows, index](const auto &x) { windows[index].settings.size = x; },
        };
    };

    auto manager = WindowManager(factory);

    auto window1 = manager.Add({
        .title = "Test1",
        .position = {.x = 1, .y = 2},
        .size = {.width = 3, .height = 4},
    });

    auto window2 = manager.Add({
        .title = "Test2",
        .position = {.x = 5, .y = 6},
        .size = {.width = 7, .height = 8},
    });

    ASSERT_NE(window1.GetNativeWindow().ptr, nullptr);
    ASSERT_EQ(window1.GetTitle(), "Test1");
    ASSERT_EQ(window1.GetPosition().x, 1);
    ASSERT_EQ(window1.GetPosition().y, 2);
    ASSERT_EQ(window1.GetSize().width, 3);
    ASSERT_EQ(window1.GetSize().height, 4);
    ASSERT_TRUE(window1.GetEvents().empty());

    ASSERT_NE(window2.GetNativeWindow().ptr, nullptr);
    ASSERT_EQ(window2.GetTitle(), "Test2");
    ASSERT_EQ(window2.GetPosition().x, 5);
    ASSERT_EQ(window2.GetPosition().y, 6);
    ASSERT_EQ(window2.GetSize().width, 7);
    ASSERT_EQ(window2.GetSize().height, 8);
    ASSERT_TRUE(window2.GetEvents().empty());

    window2.Show();
    ASSERT_FALSE(windows[0].showed);
    ASSERT_TRUE(windows[1].showed);

    window1.SetTitle("Test3");
    ASSERT_EQ(windows[0].settings.title, "Test3");
    ASSERT_EQ(window1.GetTitle(), "Test1");
    ASSERT_EQ(window2.GetTitle(), "Test2");

    windows[0].events.emplace_back(WindowRename("Test3"));
    windows[0].events.emplace_back(WindowMove{{.x = 5, .y = 6}});
    windows[1].events.emplace_back(WindowResize{{.width = 1, .height = 2}});

    manager.Poll();

    ASSERT_NE(window1.GetNativeWindow().ptr, nullptr);
    ASSERT_EQ(window1.GetTitle(), "Test3");
    ASSERT_EQ(window1.GetPosition().x, 5);
    ASSERT_EQ(window1.GetPosition().y, 6);
    ASSERT_EQ(window1.GetSize().width, 3);
    ASSERT_EQ(window1.GetSize().height, 4);
    ASSERT_EQ(window1.GetEvents().size(), 2);

    ASSERT_NE(window2.GetNativeWindow().ptr, nullptr);
    ASSERT_EQ(window2.GetTitle(), "Test2");
    ASSERT_EQ(window2.GetPosition().x, 5);
    ASSERT_EQ(window2.GetPosition().y, 6);
    ASSERT_EQ(window2.GetSize().width, 1);
    ASSERT_EQ(window2.GetSize().height, 2);
    ASSERT_EQ(window2.GetEvents().size(), 1);
}
