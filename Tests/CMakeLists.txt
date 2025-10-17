find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/Test*.cpp)

add_executable(Tests ${SOURCES})

target_include_directories(Tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(Tests PRIVATE GTest::gtest_main Core)

include(GoogleTest)

file(TO_NATIVE_PATH "/" SEPARATOR)

foreach(SOURCE ${SOURCES})
    file(RELATIVE_PATH RELATIVE_SOURCE ${CMAKE_CURRENT_LIST_DIR} ${SOURCE})
    get_filename_component(RELATIVE_DIR ${RELATIVE_SOURCE} DIRECTORY)
    string(REPLACE ${SEPARATOR} "." PREFIX ${RELATIVE_DIR})
    string(APPEND PREFIX ".")

    gtest_add_tests(
        TARGET Tests
        SOURCES ${SOURCE}
        TEST_PREFIX ${PREFIX}
    )
endforeach()
