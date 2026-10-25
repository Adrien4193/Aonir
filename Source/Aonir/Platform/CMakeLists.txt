if(WIN32)
    set(PLATFORM_NAME Win32)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

file(GLOB_RECURSE HEADERS ${PLATFORM_NAME}/*.hpp)
file(GLOB_RECURSE SOURCES ${PLATFORM_NAME}/*.cpp)

add_library(Platform)

if(WIN32)
    target_compile_definitions(Platform PUBLIC UNICODE)
endif()

configure_library(Platform "${HEADERS}" "${SOURCES}")

target_link_libraries(Platform PUBLIC Core)

install_library(
    Platform
    DESCRIPTION "Platform specific part of the engine"
    DEPENDS Core
)
