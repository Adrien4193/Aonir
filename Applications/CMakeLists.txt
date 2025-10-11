add_executable(Application)

configure_application(Application Application.cpp)

target_link_libraries(
    Application
    PRIVATE
    ${PROJECT_NAME}::Lib1
    ${PROJECT_NAME}::Lib2
)

install_application(
    Application
    DESCRIPTION "This is an application"
    DEPENDS Lib1 Lib2
)
