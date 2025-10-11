#include <print>

#include <Template/Lib1/Lib1.hpp>
#include <Template/Lib2/Lib2.hpp>

// NOLINTNEXTLINE(bugprone-exception-escape)
auto main(int argc, const char **argv) -> int
{
    std::println("argc: {}", argc);

    for (auto i = 0; i < argc; ++i)
    {
        std::println("argv [{}]: {}", i, argv[i]);
    }

    std::println("Lib1: {}", Template::Lib1(1));
    std::println("Lib2: {}", Template::Lib2(2));
}
