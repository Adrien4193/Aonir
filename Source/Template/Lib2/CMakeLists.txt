file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(Lib2)

configure_library(Lib2 "${HEADERS}" "${SOURCES}")

target_link_libraries(Lib2 Lib1)

install_library(
    Lib2
    DESCRIPTION "This is another library"
    DEPENDS Lib1
)
