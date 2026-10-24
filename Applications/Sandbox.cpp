#include <format>
#include <print>
#include <utility>
#include <variant>

#include <Aonir/Core/Utils/Logger.hpp>
#include <Aonir/Core/Utils/Overload.hpp>
#include <Aonir/Core/Utils/Version.hpp>
#include <Aonir/Core/Window/Events.hpp>
#include <Aonir/Core/Window/Keyboard.hpp>
#include <Aonir/Core/Window/Mouse.hpp>
#include <Aonir/Core/Window/Window.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

#include <Aonir/Platform/Window.hpp>

#include <Aonir/Vulkan/GraphicsApi.hpp>

using namespace Aonir;

// NOLINTBEGIN(readability-identifier-naming)

namespace std
{
    template<>
    struct formatter<Aonir::Key>
    {
        static constexpr auto parse(std::format_parse_context &ctx)
        {
            return ctx.begin();
        }

        static auto format(const Aonir::Key &key, std::format_context &ctx)
        {
            auto virtualKey = key.virtualKey.transform([](auto x) { return std::to_underlying(x); }).value_or(255);
            auto physicalKey = std::to_underlying(key.physicalKey);
            return std::format_to(ctx.out(), "({}, {})", virtualKey, physicalKey);
        }
    };
}

// NOLINTEND(readability-identifier-naming)

// NOLINTNEXTLINE(bugprone-exception-escape)
auto main(int argc, const char **argv) -> int
{
    auto logger = Logger("Sandbox", LogLevel::Debug, [](const auto &x) { std::println("{}", FormatLogEntry(x)); });

    logger.Info("argc: {}", argc);

    for (auto i = 0; i < argc; ++i)
    {
        logger.Info("argv [{}]: {}", i, argv[i]);
    }

    auto windows = WindowManager(CreateNativeWindowFactory({
        .className = "Aonir",
    }));

    auto window = windows.Add({
        .title = "Sandbox",
        .position = {.x = 1'000, .y = 500},
        .size = {.width = 1'000, .height = 500},
    });

    window.Show();

    const auto graphicsApiFactory = CreateVulkanFactory();

    const auto extensions = graphicsApiFactory.getAvailableExtensions();
    const auto layers = graphicsApiFactory.getAvailableDebugLayers();

    const auto test = graphicsApiFactory.createGraphicsApi({
        .applicationName = "Sandbox",
        .applicationVersion = Version{.major = 1, .minor = 0, .patch = 0},
        .engineName = "Aonir",
        .engineVersion = Version{.major = 1, .minor = 0, .patch = 0},
        .extensions = {},
        .debugLayers = {},
        .logger = logger,
    });

    (void)test;

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
            [&](const KeyPress &e) { logger.Info("KeyPress: {}", e.key); },
            [&](const KeyRelease &e) { logger.Info("KeyRelease: {}", e.key); },
            [&](const KeyText &e) { logger.Info("Text input: {}", e.codepoint); },
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
