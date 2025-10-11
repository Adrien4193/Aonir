include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/CMake/PackageConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${PROJECT_INSTALL_CONFIGDIR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${PROJECT_INSTALL_CONFIGDIR}
    COMPONENT Development
)

cpack_add_component(
    Development
    DISPLAY_NAME "Development"
    DESCRIPTION "CMake package files"
    INSTALL_TYPES Full Development
)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT SystemRuntime)

include(InstallRequiredSystemLibraries)

cpack_add_component(
    SystemRuntime
    DISPLAY_NAME "System runtime"
    DESCRIPTION "System runtime libraries"
    INSTALL_TYPES Full Runtime
)
