file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(Lib1)

configure_library(Lib1 "${HEADERS}" "${SOURCES}")

install_library(
    Lib1
    DESCRIPTION "This is a library"
)
