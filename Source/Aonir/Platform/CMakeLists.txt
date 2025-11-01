if(WIN32)
    set(PLATFORM_NAME Win32)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

file(GLOB HEADERS *.hpp)
file(GLOB_RECURSE SOURCES ${PLATFORM_NAME}/*.cpp ${PLATFORM_NAME}/*.hpp)

add_library(Platform)

configure_library(Platform "${HEADERS}" "${SOURCES}")

target_link_libraries(Platform PUBLIC Core)

install_library(
    Platform
    DESCRIPTION "Platform specific part of the engine"
    DEPENDS Core
)
