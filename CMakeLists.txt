cmake_minimum_required(VERSION 4.0.3)

project(
    Template
    VERSION 0.1.0
    DESCRIPTION "C++ template"
    HOMEPAGE_URL https://github.com/Adrien4193/CppTemplate
    LANGUAGES CXX
)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_APPLICATIONS "Build application" ON)
option(BUILD_TESTS "Build tests" ON)

list(PREPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

include(CompileOptions)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(GNUInstallDirs)

set(PROJECT_INSTALL_CONFIGDIR cmake)

include(PackageOptions)

include(Application)
include(Library)

add_subdirectory(Source/${PROJECT_NAME})

if(BUILD_APPLICATIONS)
    add_subdirectory(Applications)
endif()

if(BUILD_TESTS)
    include(CTest)
    add_subdirectory(Tests)
endif()

include(Package)
