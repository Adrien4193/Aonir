include(ExportMacro)

function(configure_library TARGET HEADERS SOURCES)
    add_library(${PROJECT_NAME}::${TARGET} ALIAS ${TARGET})

    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${TARGET})

    target_sources(
        ${TARGET}
        PUBLIC FILE_SET HEADERS FILES ${HEADERS}
        PRIVATE ${SOURCES}
    )

    target_include_directories(
        ${TARGET}
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    define_export_macro(${TARGET})
endfunction()

function(install_library TARGET)
    cmake_parse_arguments(arg "" "DESCRIPTION" "DEPENDS" ${ARGN})

    install(
        TARGETS ${TARGET}
        EXPORT ${TARGET}Targets
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${TARGET}Runtime
        LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${TARGET}Runtime
        NAMELINK_COMPONENT ${TARGET}Development
        ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${TARGET}Development
        FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${TARGET}
        COMPONENT ${TARGET}Development
    )

    install(
        EXPORT ${TARGET}Targets
        FILE ${PROJECT_NAME}${TARGET}Targets.cmake
        DESTINATION ${PROJECT_INSTALL_CONFIGDIR}
        NAMESPACE ${PROJECT_NAME}::
        COMPONENT ${TARGET}Development
    )

    set(RUNTIME_DEPENDENCIES "")
    set(DEVELOPMENT_DEPENDENCIES "")

    foreach(DEPENDENCY ${arg_DEPENDS})
        list(APPEND RUNTIME_DEPENDENCIES ${DEPENDENCY}Runtime)
        list(APPEND DEVELOPMENT_DEPENDENCIES ${DEPENDENCY}Development)
    endforeach()

    cpack_add_component_group(
        ${TARGET}
        DISPLAY_NAME ${TARGET}
        DESCRIPTION ${arg_DESCRIPTION}
    )

    get_target_property(TARGET_TYPE ${TARGET} TYPE)

    if(${TARGET_TYPE} STREQUAL SHARED_LIBRARY)
        cpack_add_component(
            ${TARGET}Runtime
            DISPLAY_NAME "${TARGET} runtime"
            DESCRIPTION "Runtime artifacts"
            GROUP ${TARGET}
            DEPENDS ${RUNTIME_DEPENDENCIES}
            INSTALL_TYPES Full Runtime Development
        )
    endif()

    cpack_add_component(
        ${TARGET}Development
        DISPLAY_NAME "${TARGET} development"
        DESCRIPTION "Development artifacts"
        GROUP ${TARGET}
        DEPENDS Development ${TARGET}Runtime ${DEVELOPMENT_DEPENDENCIES}
        INSTALL_TYPES Full Development
    )
endfunction()
