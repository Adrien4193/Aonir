find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/Test*.cpp)

add_executable(Tests ${SOURCES})

target_include_directories(Tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(Tests PRIVATE GTest::gtest_main Lib1 Lib2)

include(GoogleTest)
gtest_add_tests(TARGET Tests)
