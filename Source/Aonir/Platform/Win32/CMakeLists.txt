file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(Platform)

configure_library(Platform "${HEADERS}" "${SOURCES}")

target_link_libraries(Platform PUBLIC Core)

install_library(
    Platform
    DESCRIPTION "Platform specific part of the engine"
    DEPENDS Core
)
