add_library(Platform)

file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)

configure_library(Platform "${HEADERS}" "${SOURCES}")

target_link_libraries(Platform PUBLIC Core)

target_compile_definitions(Platform PUBLIC UNICODE)

install_library(
    Platform
    DESCRIPTION "Win32 specific part of the engine"
    DEPENDS Core
)
