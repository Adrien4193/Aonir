add_executable(Sandbox)

configure_application(Sandbox Sandbox.cpp)

target_link_libraries(
    Sandbox
    PRIVATE
    Core
    Platform
)
