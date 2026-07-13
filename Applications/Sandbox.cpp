#include <print>
#include <variant>

#include <Aonir/Core/Utils/Logger.hpp>
#include <Aonir/Core/Utils/Overload.hpp>
#include <Aonir/Core/Window/WindowEvents.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

#include <Aonir/Platform/WindowFactory.hpp>

using namespace Aonir;

// NOLINTNEXTLINE(bugprone-exception-escape)
auto main(int argc, const char **argv) -> int
{
    auto logger = Logger("Sandbox", LogLevel::Debug, [](const auto &record) { std::println("{}", FormatLogEntry(record)); });

    logger.Info("argc: {}", argc);

    for (auto i = 0; i < argc; ++i)
    {
        logger.Info("argv [{}]: {}", i, argv[i]);
    }

    auto windows = WindowManager(CreateNativeWindowFactory());

    auto window = windows.Add({
        .title = "Sandbox",
        .position = {.x = 1'000, .y = 500},
        .size = {.width = 1'000, .height = 500},
    });

    window.Show();

    auto closed = false;

    while (!closed)
    {
        logger.Trace("Update");

        windows.Poll();

        auto handler = Overload{
            [&](const WindowError &) { logger.Error("WindowError"); },
            [&](const WindowRename &e) { logger.Info("WindowRename: {}", e.title); },
            [&](const WindowMove &e) { logger.Info("WindowMove: {} {}", e.position.x, e.position.y); },
            [&](const WindowResize &e) { logger.Info("WindowResize: {} {}", e.size.width, e.size.height); },
            [&](const WindowClose &) { closed = true; },
            [&](const KeyPress &e) { logger.Info("KeyPress: {}", static_cast<int>(e.key)); },
            [&](const KeyRelease &e) { logger.Info("KeyRelease: {}", static_cast<int>(e.key)); },
            [&](const TextInput &e) { logger.Info("Text input: {}", e.codepoint); },
            [&](const MouseButtonPress &e) { logger.Info("MouseButtonPress: {}", static_cast<int>(e.button)); },
            [&](const MouseButtonRelease &e) { logger.Info("MouseButtonRelease: {}", static_cast<int>(e.button)); },
            [&](const MouseMove &e) { logger.Info("MouseMove: {} {}", e.position.x, e.position.y); },
            [&](const MouseScroll &e) { logger.Info("MouseScroll: {} {}", e.delta.x, e.delta.y); },
        };

        for (const auto &e : window.GetEvents())
        {
            std::visit(handler, e);
        }
    }

    logger.Info("Window closed");
}
