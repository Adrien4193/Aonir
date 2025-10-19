find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(Core)

configure_library(Core "${HEADERS}" "${SOURCES}")

target_link_libraries(Core PUBLIC glm::glm)

install_library(
    Core
    DESCRIPTION "Core features of the engine"
)
