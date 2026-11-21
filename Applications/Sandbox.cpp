#include <array>
#include <format>
#include <print>
#include <utility>
#include <variant>

#include <Aonir/Core/Utils/Logger.hpp>
#include <Aonir/Core/Utils/Overload.hpp>
#include <Aonir/Core/Utils/Version.hpp>
#include <Aonir/Core/Window/Window.hpp>
#include <Aonir/Core/Window/WindowEvent.hpp>
#include <Aonir/Core/Window/WindowManager.hpp>

#include <Aonir/Platform/Win32/Window/WindowFactory.hpp>

#include <Aonir/Vulkan/Instance.hpp>

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

    auto windows = WindowManager(CreateWin32WindowFactory({.name = "Aonir"}));

    auto window = windows.Add({
        .title = "Sandbox",
        .position = {.x = 1'000, .y = 500},
        .size = {.width = 1'000, .height = 500},
    });

    window.Show();

    const auto availableExtensions = GetAvailableVulkanExtensions();
    const auto availableLayers = GetAvailableVulkanValidationLayers();

    auto instance = CreateVulkanInstance({
        .applicationName = "Sandbox",
        .applicationVersion = Version{.major = 1, .minor = 0, .patch = 0},
        .engineName = "Aonir",
        .engineVersion = Version{.major = 1, .minor = 0, .patch = 0},
        .extensions = {&vulkanDebugExtensionName, 1},
        .validationLayers = {&khronosValidationLayer, 1},
        .logger = &logger,
    });

    auto physicalDevices = instance.GetAvailablePhysicalDevices();

    auto queuePriority = 1.0F;
    auto queue = VkDeviceQueueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = 0,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority,
    };

    const auto device = instance.CreateDevice({
        .physicalDevice = physicalDevices[0].handle,
        .extensions = {},
        .queues = {&queue, 1},
        .features = VkPhysicalDeviceFeatures(),
    });

    (void)device;

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
