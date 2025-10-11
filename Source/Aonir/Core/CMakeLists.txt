file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(Core)

configure_library(Core "${HEADERS}" "${SOURCES}")

install_library(
    Core
    DESCRIPTION "This is a library"
)
