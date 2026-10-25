file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)

find_package(Vulkan REQUIRED)

add_library(Vulkan)

configure_library(Vulkan "${HEADERS}" "${SOURCES}")

target_link_libraries(Vulkan PUBLIC Core)
target_link_libraries(Vulkan PUBLIC Vulkan::Vulkan)

install_library(
    Vulkan
    DESCRIPTION "Vulkan renderer"
    DEPENDS Core
)
