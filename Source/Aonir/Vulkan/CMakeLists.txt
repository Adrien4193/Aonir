add_library(Vulkan)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE HEADERS Core/*.hpp Platform/${AONIR_PLATFORM}/*.hpp)
file(GLOB_RECURSE SOURCES Core/*.cpp Platform/${AONIR_PLATFORM}/*.cpp)

configure_library(Vulkan "${HEADERS}" "${SOURCES}")

target_link_libraries(Vulkan PUBLIC Core)
target_link_libraries(Vulkan PUBLIC Vulkan::Vulkan)

install_library(
    Vulkan
    DESCRIPTION "Vulkan renderer"
    DEPENDS Core
)
